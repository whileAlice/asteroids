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
#define UNKNOWN_THREAD THREAD_COUNT

static Error* s_errors[THREAD_COUNT + 1] = {
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

   va_end (args_copy);

   abort ();
}

void
error_print (Error* err)
{
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

   if (errno != 0)
   {
      // TODO: sb_appendf
      sb_append (sb, " (errno: ");
      sb_append (sb, strerror (errno));
      sb_append (sb, ")");
   }

   log_error (sb->data);

   sb_free (sb);
}

void
errors_print (void)
{
   size_t errors_len = sizeof (s_errors) / sizeof (s_errors[0]);
   for (size_t i = 0; i < errors_len; ++i)
   {
      error_print (s_errors[i]);
   }
}

void
errors_free (void)
{
   for (size_t i = 0; i < THREAD_COUNT; ++i)
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
