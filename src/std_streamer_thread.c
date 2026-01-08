#include "std_streamer_thread.h"

#include "context.h"
#include "error.h"
#include "log.h"
#include "threads.h"

#include <assert.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

#define LOG_DEBUG_DUP(fmt, ...)                                        \
   log_to_file (stderr_context.write_file, DEBUG_LOG_LEVEL, true, fmt, \
                ##__VA_ARGS__)

#define LOG_ERROR_DUP(fmt, ...)                                        \
   log_to_file (stderr_context.write_file, ERROR_LOG_LEVEL, true, fmt, \
                ##__VA_ARGS__)

#define BUFFER_SIZE 4096

void*
std_streamer_thread (void* arg)
{
   Context* c = (Context*)arg;
   Log*     l = c->log;

   int     err;
   ssize_t length;
   char    buf[BUFFER_SIZE];

   int stdout_fd_copy;
   int stderr_fd_copy;
   int epoll_fd;

   if (pipe (l->stdout_pipe) == -1)
      ERROR_GOTO (exit, "stdout pipe");
   if (pipe (l->stderr_pipe) == -1)
      ERROR_GOTO (exit, "stderr pipe");

   StreamContext stdout_context = { .read_fd = l->stdout_pipe[READ_END],
                                    .name    = "stdout" };
   StreamContext stderr_context = { .read_fd = l->stderr_pipe[READ_END],
                                    .name    = "stderr" };
   StreamContext wakeup_context = { .read_fd = l->wakeup_pipe[READ_END],
                                    .name    = "wakeup" };

   struct epoll_event all_events[EVENT_COUNT] = {
      [STDOUT_EVENT] = { .data.ptr = &stdout_context },
      [STDERR_EVENT] = { .data.ptr = &stderr_context },
      [WAKEUP_EVENT] = { .data.ptr = &wakeup_context },
   };
   struct epoll_event ready_events[EVENT_COUNT];

   // backup std stream fds
   stdout_fd_copy = dup (STDOUT_FILENO);
   if (stdout_fd_copy == -1)
      ERROR_GOTO (exit, "stdout fd copy dup");
   stderr_fd_copy = dup (STDERR_FILENO);
   if (stderr_fd_copy == -1)
      ERROR_GOTO (exit, "stderr fd copy dup");

   // open backups for writing
   stdout_context.write_file = fdopen (stdout_fd_copy, "w");
   if (stdout_context.write_file == NULL)
      ERROR_GOTO (exit, "stdout copy fdopen");
   stderr_context.write_file = fdopen (stderr_fd_copy, "w");
   if (stderr_context.write_file == NULL)
      ERROR_GOTO (exit, "stderr copy fdopen");

   // replace original std stream fds with pipe write ends
   err = dup2 (l->stderr_pipe[WRITE_END], STDERR_FILENO);
   if (err == -1)
      ERROR_GOTO (exit, "stderr pipe dup2");
   err = dup2 (l->stdout_pipe[WRITE_END], STDOUT_FILENO);
   if (err == -1)
      ERROR_GOTO (restore_streams, "stdout pipe dup2");

   epoll_fd = epoll_create1 (0);
   if (epoll_fd == -1)
      ERROR_GOTO (restore_streams, "epoll create");

   // poll all pipe read ends
   for (size_t i = 0; i < EVENT_COUNT; ++i)
   {
      StreamContext* c     = all_events[i].data.ptr;
      all_events[i].events = EPOLLIN;

      err = epoll_ctl (epoll_fd, EPOLL_CTL_ADD, c->read_fd, &all_events[i]);
      if (err == -1)
         ERROR_GOTO (restore_streams, "epoll ctl add %s", c->name);
   }

   LOG_DEBUG_DUP ("syncing...");
   SYNC_THREAD (&l->mutex, &l->cond, l->thread_ready_count,
                STREAMER_THREAD_COUNT, l->should_abort_init, restore_streams);

   LOG_DEBUG_DUP ("ready!");

   int ready_count;
   while (true)
   {
      IN_LOCK(&c->app->mutex,
      {
         if (c->app->should_quit)
         {
            pthread_mutex_unlock(&c->app->mutex);
            goto restore_streams;
         }
      });

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

         length = read (c->read_fd, buf, sizeof (buf) - 1);
         switch (length)
         {
         case -1: ERROR_GOTO (restore_streams, "%s read", c->name);
         case 0 : ERROR_GOTO (restore_streams, "%s EOF", c->name);
         default: buf[length] = '\0';
         }

         if (c->write_file != NULL)
         {
            assert (i < INTERNAL_LOG);
            IN_LOCK(&l->mutex,
               log_buffer_write_string (l->buffers[i], buf);
            );
            fputs (buf, c->write_file);
         }
         else
            LOG_DEBUG_DUP ("received data from %s pipe: %s", c->name, buf);
      }
   }

restore_streams:
   LOG_DEBUG_DUP ("restoring streams...");

   // restore std stream fds from backups
   err = dup2 (stdout_fd_copy, STDOUT_FILENO);
   if (err == -1)
   {
      LOG_ERROR_DUP ("stdout fd copy dup2");
      goto exit;
   }
   err = dup2 (stdout_fd_copy, STDERR_FILENO);
   if (err == -1)
   {
      LOG_ERROR_DUP ("stderr fd copy dup2");
      goto exit;
   }

   // close backups
   err = close (stdout_fd_copy);
   if (err == -1)
      ERROR_GOTO (exit, "stdout fd copy close");

   err = close (stderr_fd_copy);
   if (err == -1)
      ERROR_GOTO (exit, "stderr fd copy close");

exit:
   log_debug ("returning...");

   IN_LOCK(&c->app->mutex,
   {
      c->app->should_quit = true;
      pthread_cond_broadcast (&c->app->cond);
   });

   return NULL;
}
