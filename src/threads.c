#include "threads.h"

#include "context.h"
#include "std_streamer_thread.h"

static pthread_t s_std_streamer_thread_id;

int
threads_init (Context* c)
{
   pthread_mutex_init (&c->log->mutex, NULL);
   pthread_mutex_init (&c->state->mutex, NULL);
   pthread_cond_init (&c->log->cond, NULL);
   pthread_cond_init (&c->state->cond, NULL);

   int err = pthread_create (&s_std_streamer_thread_id, NULL,
                             std_streamer_thread, (void*)c);

   // TODO: maybe handle this a bit more here
   if (err != 0)
   {
      perror ("pthread create");
      return err;
   }

   return 0;
}

int
threads_deinit (Context* c)
{
   int err;

   err = pthread_join (s_std_streamer_thread_id, NULL);
   if (err != 0)
   {
      perror ("pthread join");
      return err;
   }

   err = pthread_mutex_destroy (&c->log->mutex);
   if (err != 0)
   {
      perror ("pthread mutex destroy");
      return err;
   }
   err = pthread_mutex_destroy (&c->state->mutex);
   if (err != 0)
   {
      perror ("pthread mutex destroy");
      return err;
   }

   err = pthread_cond_destroy (&c->log->cond);
   if (err != 0)
   {
      perror ("pthread cond destroy");
      return err;
   }
   err = pthread_cond_destroy (&c->state->cond);
   if (err != 0)
   {
      perror ("pthread cond destroy");
      return err;
   }

   return 0;
}
