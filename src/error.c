#include "error.h"

#include "log.h"
#include "threads.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// adding one more entry to account for the possibility of a thread
// erroring before its ID is obtained and stored by the main thread
#define ERROR_COUNT THREAD_COUNT + 1

static Error* s_errors[ERROR_COUNT] = {
   [MAIN_THREAD]     = NULL,
   [STREAMER_THREAD] = NULL,
   [UNKNOWN_THREAD]  = NULL,
};

static pthread_mutex_t s_error_mutex = PTHREAD_MUTEX_INITIALIZER;

void
error_set (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   Error**   prev_error;
   ThreadIdx thread_idx = get_thread_idx (pthread_self ());

   pthread_mutex_lock (&s_error_mutex);

   if ((int)thread_idx == -1)
      prev_error = &s_errors[UNKNOWN_THREAD];
   else
      prev_error = &s_errors[thread_idx];

   Error* new_error = malloc (sizeof (Error));
   if (new_error == NULL)
   {
      log_error ("set error malloc");
      goto fail;
   }

   new_error->message = vstrdupf (fmt, args);
   if (new_error->message == NULL)
   {
      log_error ("set error vstrdupf");
      goto fail;
   }

   va_end (args);

   new_error->next = *prev_error;
   *prev_error     = new_error;

   pthread_mutex_unlock (&s_error_mutex);

   return;

fail:
   va_list args_copy;
   va_start (args_copy, fmt);

   vlog_to_file (stderr, ERROR_LOG_LEVEL, fmt, args_copy);
   log_to_file (stderr, ERROR_LOG_LEVEL, ERRNO_FORMAT, errno, strerror (errno));

   va_end (args_copy);

   abort ();
}

void
error_set_with_errno (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   StringBuilder* sb_err = sb_vcreatef (fmt, args);
   if (sb_err == NULL)
   {
      log_error ("error set with errno sb vcreatef");
      goto fail;
   }

   va_end (args);

   if (!sb_appendf (sb_err, " (" ERRNO_FORMAT ")", errno, strerror (errno)))
   {
      log_error ("error set with errno sb appendf");
      goto fail;
   }

   error_set (sb_err->data);

   sb_free (sb_err);
   return;

fail:
   va_list args_copy;
   va_start (args_copy, fmt);

   vlog_to_file (stderr, ERROR_LOG_LEVEL, fmt, args_copy);
   log_to_file (stderr, ERROR_LOG_LEVEL, ERRNO_FORMAT, errno, strerror (errno));

   va_end (args_copy);

   abort ();
}

void
error_print (ThreadIdx thread_idx)
{
   assert (thread_idx >= 0);
   assert (thread_idx <= UNKNOWN_THREAD);

   Error* err = s_errors[thread_idx];

   if (err == NULL)
      return;

   StringBuilder* sb = sb_create (NULL);

   while (err != NULL)
   {
      sb_append (sb, err->message);
      if (err->next != NULL)
         sb_append (sb, ": ");

      err = err->next;
   }

   log_error (sb->data);

   sb_free (sb);
}

void
errors_free (void)
{
   for (size_t i = 0; i < ERROR_COUNT; ++i)
   {
      Error* current = s_errors[i];
      Error* next;

      while (current != NULL)
      {
         next = current->next;
         free (current->message);
         free (current);
         current = next;
      }

      s_errors[i] = NULL;
   }
}

bool
has_error (ThreadIdx thread_idx)
{
   assert (thread_idx >= 0);
   assert (thread_idx <= UNKNOWN_THREAD);

   if (s_errors[thread_idx] != NULL)
      return true;

   return false;
}
