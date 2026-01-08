#include "std_streamer_thread.h"

#include "context.h"
#include "log.h"
#include "threads.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#define DEBUG_DUP(fmt, ...)                                            \
   log_to_file (stderr_context.write_file, DEBUG_LOG_LEVEL, true, fmt, \
                ##__VA_ARGS__)

// TODO: after implementing proper error functions,
// we shouldn't need no-newline support in log_to_file.
// Also, this must go, because there's a risk of errno
// string/newline interleaving with other threads.
#define PERROR_DUP(fmt, ...)                                               \
   do                                                                      \
   {                                                                       \
      log_to_file (stderr_context.write_file, ERROR_LOG_LEVEL, false, fmt, \
                   ##__VA_ARGS__);                                         \
      if (errno)                                                           \
         fprintf (stderr_context.write_file, ": %s", strerror (errno));    \
                                                                           \
      fputc ('\n', stderr_context.write_file);                             \
   }                                                                       \
   while (0)

#define FAIL_GOTO(fmt, ...)           \
   do                                 \
   {                                  \
      error_set (fmt, ##__VA_ARGS__); \
      goto exit;                      \
   }                                  \
   while (0)

#define FAIL_DUP(fmt, ...)             \
   do                                  \
   {                                   \
      PERROR_DUP (fmt, ##__VA_ARGS__); \
      goto restore_streams;            \
   }                                   \
   while (0)

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
      FAIL_GOTO ("stdout pipe");
   if (pipe (l->stderr_pipe) == -1)
      FAIL_GOTO ("stderr pipe");

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
      FAIL_GOTO ("stdout fd copy dup");
   stderr_fd_copy = dup (STDERR_FILENO);
   if (stderr_fd_copy == -1)
      FAIL_GOTO ("stderr fd copy dup");

   // open backups for writing
   stdout_context.write_file = fdopen (stdout_fd_copy, "w");
   if (stdout_context.write_file == NULL)
      FAIL_GOTO ("stdout copy fdopen");
   stderr_context.write_file = fdopen (stderr_fd_copy, "w");
   if (stderr_context.write_file == NULL)
      FAIL_GOTO ("stderr copy fdopen");

   // replace original std stream fds with pipe write ends
   err = dup2 (l->stderr_pipe[WRITE_END], STDERR_FILENO);
   if (err == -1)
      FAIL_GOTO ("stderr pipe dup2");
   err = dup2 (l->stdout_pipe[WRITE_END], STDOUT_FILENO);
   if (err == -1)
      FAIL_DUP ("stdout pipe dup2");

   epoll_fd = epoll_create1 (0);
   if (epoll_fd == -1)
      FAIL_DUP ("epoll create");

   // poll all pipe read ends
   for (size_t i = 0; i < EVENT_COUNT; ++i)
   {
      StreamContext* c     = all_events[i].data.ptr;
      all_events[i].events = EPOLLIN;

      err = epoll_ctl (epoll_fd, EPOLL_CTL_ADD, c->read_fd, &all_events[i]);
      if (err == -1)
         FAIL_DUP ("epoll ctl add %s", c->name);
   }

   DEBUG_DUP ("syncing...");
   SYNC_THREAD (&l->mutex, &l->cond, l->thread_ready_count,
                STREAMER_THREAD_COUNT, l->should_abort_init, restore_streams);

   DEBUG_DUP ("ready!");

   int ready_count;
   while (true)
   {
      IN_LOCK(&c->app->mutex,
         if (c->app->should_quit)
         {
            pthread_mutex_unlock(&c->app->mutex);
            goto restore_streams;
         }
      );

      ready_count = epoll_wait (epoll_fd, ready_events, EVENT_COUNT, -1);
      if (ready_count == -1)
         FAIL_DUP ("epoll wait");

      for (int i = 0; i < ready_count; ++i)
      {
         StreamContext* c = ready_events[i].data.ptr;

         if (ready_events[i].events & EPOLLHUP)
            FAIL_DUP ("%s hang up", c->name);

         if (ready_events[i].events & EPOLLERR)
            FAIL_DUP ("%s fd error", c->name);

         assert (ready_events[i].events & EPOLLIN);

         length = read (c->read_fd, buf, sizeof (buf) - 1);
         switch (length)
         {
         case -1: FAIL_DUP ("%s read", c->name);
         case 0 : FAIL_DUP ("%s EOF", c->name);
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
            DEBUG_DUP ("received data from %s pipe: %s", c->name, buf);
      }
   }

restore_streams:
   DEBUG_DUP ("restoring streams...");

   // restore std stream fds from backups
   err = dup2 (stdout_fd_copy, STDOUT_FILENO);
   if (err == -1)
      PERROR_DUP ("stdout fd copy dup2");
   err = dup2 (stdout_fd_copy, STDERR_FILENO);
   if (err == -1)
      PERROR_DUP ("stderr fd copy dup2");

   // close backups
   err = close (stdout_fd_copy);
   if (err == -1)
   {
      perror ("stdout fd copy close");
      goto exit;
   }
   err = close (stderr_fd_copy);
   if (err == -1)
   {
      perror ("stderr fd copy close");
      goto exit;
   }

exit:
   log_debug ("returning...");

   IN_LOCK(&c->app->mutex,
   {
      c->app->should_quit = true;
      pthread_cond_broadcast(&c->app->cond);
   });

   return NULL;
}
