#include "threads.h"

#include "context.h"
#include "std_streamer_thread.h"

#include <assert.h>

static pthread_t s_thread_ids[THREAD_COUNT];

int
threads_init (Context* c)
{
   // this assumes threads_init is run by the main thread
   s_thread_ids[MAIN_THREAD] = pthread_self ();

   pthread_mutex_init (&c->log->mutex, NULL);
   pthread_mutex_init (&c->app->mutex, NULL);
   pthread_cond_init (&c->log->cond, NULL);
   pthread_cond_init (&c->app->cond, NULL);

   int err = pthread_create (&s_thread_ids[STREAMER_THREAD], NULL,
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

   err = pthread_join (s_thread_ids[STREAMER_THREAD], NULL);
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
   err = pthread_mutex_destroy (&c->app->mutex);
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
   err = pthread_cond_destroy (&c->app->cond);
   if (err != 0)
   {
      perror ("pthread cond destroy");
      return err;
   }

   return 0;
}

ThreadIdx
get_thread_idx (pthread_t thread_id)
{
   for (size_t i = 0; i < THREAD_COUNT; ++i)
      if pthread_equal (s_thread_ids[i], thread_id)
         return (ThreadIdx)i;

   return -1;
}
