#include "std_streamer_thread.h"

#include "context.h"
#include "error.h"
#include "log.h"
#include "threads.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

#define LOG_DEBUG_DUP(fmt, ...)                                 \
   log_to_file (stream_contexts[STDERR_EVENT].stream_file_copy, \
                DEBUG_LOG_LEVEL, fmt, ##__VA_ARGS__)

#define LOG_ERROR_DUP(fmt, ...)                                 \
   log_to_file (stream_contexts[STDERR_EVENT].stream_file_copy, \
                ERROR_LOG_LEVEL, fmt, ##__VA_ARGS__)

#define BUFFER_SIZE 4096

void*
std_streamer_thread (void* arg)
{
   Context* c = (Context*)arg;
   Log*     l = c->log;

   int     err;
   ssize_t length;
   char    buf[BUFFER_SIZE];

   int                epoll_fd;
   struct epoll_event ready_events[EVENT_COUNT];

   StreamContext stream_contexts[EVENT_COUNT] = {
      [STDOUT_EVENT] = {
         .name                 = "stdout",
         .stream_fd_original   = STDOUT_FILENO,
         .stream_file_original = stdout,
      },
      [STDERR_EVENT] = {
         .name                 = "stderr",
         .stream_fd_original   = STDERR_FILENO,
         .stream_file_original = stderr,
      },
      // wakeup pipe is managed by the main thread
      [WAKEUP_EVENT]  = {
         .name = "wakeup",
         .pipe = { l->wakeup_pipe[READ_END],
                   l->wakeup_pipe[WRITE_END] },
      },
   };

   // prepare pipes that will forward std output streams
   for (size_t i = 0; i <= STDERR_EVENT; ++i)
   {
      err = pipe2 (stream_contexts[i].pipe, O_NONBLOCK);
      if (err == -1)
         ERROR_GOTO (exit, "%s pipe", stream_contexts[i].name);
   }

   // backup std stream fds
   for (size_t i = 0; i <= STDERR_EVENT; ++i)
   {
      stream_contexts[i].stream_fd_copy =
         dup (stream_contexts[i].stream_fd_original);
      if (stream_contexts[i].stream_fd_copy == -1)
         ERROR_GOTO (exit, "%s stream fd copy dup", stream_contexts[i].name);
   }

   // open backups for writing
   for (size_t i = 0; i <= STDERR_EVENT; ++i)
   {
      stream_contexts[i].stream_file_copy =
         fdopen (stream_contexts[i].stream_fd_copy, "w");
      if (stream_contexts[i].stream_file_copy == NULL)
         ERROR_GOTO (exit, "%s stream file copy fdopen",
                     stream_contexts[i].name);
   }

   // replace original std stream fds with pipe write ends
   for (size_t i = 0; i <= STDERR_EVENT; ++i)
   {
      err = dup2 (stream_contexts[i].pipe[WRITE_END],
                  stream_contexts[i].stream_fd_original);
      if (err == -1)
         ERROR_GOTO (restore_streams, "%s pipe write end dup2");
   }

   epoll_fd = epoll_create1 (0);
   if (epoll_fd == -1)
      ERROR_GOTO (restore_streams, "epoll create");

   // prepare all pipe read ends for polling
   for (size_t i = 0; i < EVENT_COUNT; ++i)
   {
      struct epoll_event event = { .events   = EPOLLIN,
                                   .data.ptr = &stream_contexts[i] };

      err = epoll_ctl (epoll_fd, EPOLL_CTL_ADD,
                       stream_contexts[i].pipe[READ_END], &event);
      if (err == -1)
         ERROR_GOTO (restore_streams, "epoll ctl add %s",
                     stream_contexts[i].name);
   }

   // wait for main
   LOG_DEBUG_DUP ("syncing...");
   SYNC_THREAD (&l->mutex, &l->cond, l->thread_ready_count,
                STREAMER_THREAD_COUNT, l->should_abort_init, restore_streams);

   LOG_DEBUG_DUP ("ready!");

   int ready_count;
   while (true)
   {
      if (c->app->should_quit)
         goto restore_streams;

      // poll all pipe read ends
      ready_count = epoll_wait (epoll_fd, ready_events, EVENT_COUNT, -1);
      if (ready_count == -1)
         ERROR_GOTO (restore_streams, "epoll wait");

      for (int i = 0; i < ready_count; ++i)
      {
         StreamContext* c = ready_events[i].data.ptr;

         if (ready_events[i].events & EPOLLHUP)
            ERROR_GOTO (restore_streams, "%s hang up", c->name);

         if (ready_events[i].events & EPOLLERR)
            ERROR_GOTO (restore_streams, "%s fd error", c->name);

         assert (ready_events[i].events & EPOLLIN);

         length = read (c->pipe[READ_END], buf, sizeof (buf) - 1);
         switch (length)
         {
         case -1:
            if (errno == EAGAIN)
            {
               errno = 0;
               continue;
            }
            ERROR_GOTO (restore_streams, "%s read", c->name);
         case 0 : ERROR_GOTO (restore_streams, "%s EOF", c->name);
         default: buf[length] = '\0';
         }

         // write forwarded std streams to buffer
         // and print them to the terminal
         if (c->stream_file_copy != NULL)
         {
            assert (i < INTERNAL_LOG);

            fputs (buf, c->stream_file_copy);
            IN_LOCK(&l->mutex,
               log_buffer_write_string (l->buffers[i], buf);
            );
         }
         else
            LOG_DEBUG_DUP ("received data from %s pipe: %s", c->name, buf);
      }
   }

restore_streams:
   LOG_DEBUG_DUP ("restoring streams...");

   // restore std stream fds from backups
   for (size_t i = 0; i <= STDERR_EVENT; ++i)
   {
      err = dup2 (stream_contexts[i].stream_fd_copy,
                  stream_contexts[i].stream_fd_original);
      if (err == -1)
      {
         LOG_ERROR_DUP ("%s fd copy dup2", stream_contexts[i].name);
         goto exit;
      }
   }

   // check stream pipes if something was sent in between
   // quit initiation and stream restoration
   for (size_t i = 0; i <= STDERR_EVENT; ++i)
   {
      length = read (stream_contexts[i].pipe[READ_END], buf, sizeof (buf) - 1);
      switch (length)
      {
      case -1:
         if (errno == EAGAIN)
         {
            errno = 0;
            continue;
         }
         ERROR_GOTO (exit, "%s read", stream_contexts[i].name);
      case 0 : ERROR_GOTO (exit, "%s EOF", stream_contexts[i].name);
      default: buf[length] = '\0';
      }

      IN_LOCK(&l->mutex,
         log_buffer_write_string (l->buffers[i], buf);
      );
      fputs (buf, stream_contexts[i].stream_file_original);
   }

   // close backups
   for (size_t i = 0; i <= STDERR_EVENT; ++i)
   {
      err = close (stream_contexts[i].stream_fd_copy);
      if (err == -1)
         ERROR_GOTO (exit, "%s stream fd copy close", stream_contexts[i].name);
   }

exit:
   log_debug ("returning...");

   // if exiting due to own error, bring down the rest of the app as well
   IN_LOCK(&c->app->mutex,
   {
      c->app->should_quit = true;
      pthread_cond_broadcast (&c->app->cond);
   });

   return NULL;
}
