#include "error.h"

#include "context.h"
#include "threads.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

static Error* s_errors[THREAD_COUNT] = {
   [MAIN_THREAD]     = NULL,
   [STREAMER_THREAD] = NULL,
};

static App* s_app;

void
errors_init (App* app)
{
   assert (app != NULL);
   s_app = app;
}

void
errors_deinit (void)
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

   s_app = NULL;
}

void
set_error (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   if (s_app == NULL)
   {
      vfprintf (stderr, fmt, args);

      if (errno != 0)
         fprintf (stderr, ": %s", strerror (errno));

      fputc ('\n', stderr);

      abort ();
   }



   ThreadIdx thread_idx = get_thread_idx (pthread_self ());
   Error*    error      = s_errors[thread_idx];

   pthread_mutex_lock (&s_app->mutex);

end:
   s_app->should_quit = true;
   pthread_cond_broadcast (&s_app->cond);

   pthread_mutex_unlock (&s_app->mutex);
}
