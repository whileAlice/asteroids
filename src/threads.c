#include "threads.h"

#include "context.h"
#include "std_streamer_thread.h"

#include <assert.h>
#include <errno.h>

#define ERRNO_SET_RETURN(retval, err, fmt, ...)  \
   do                                            \
   {                                             \
      errno = err;                               \
      ERRNO_RETURN (retval, fmt, ##__VA_ARGS__); \
   }                                             \
   while (0)

static pthread_t s_thread_ids[THREAD_COUNT];

static const char* thread_names[THREAD_COUNT] = {
   [MAIN_THREAD]     = "main",
   [STREAMER_THREAD] = "streamer",
};

bool
threads_init (Context* c)
{
   // this assumes threads_init is run by the main thread
   s_thread_ids[MAIN_THREAD] = pthread_self ();

   pthread_mutex_init (&c->log->mutex, NULL);
   pthread_mutex_init (&c->event->mutex, NULL);
   pthread_cond_init (&c->log->cond, NULL);
   pthread_cond_init (&c->event->cond, NULL);

   int err = pthread_create (&s_thread_ids[STREAMER_THREAD], NULL,
                             std_streamer_thread, (void*)c);
   if (err != 0)
      ERRNO_SET_RETURN (false, err, "pthread create");

   return true;
}

bool
threads_deinit (Context* c)
{
   int err;

   err = pthread_join (s_thread_ids[STREAMER_THREAD], NULL);
   if (err != 0)
      ERRNO_SET_RETURN (false, err, "pthread join");

   err = pthread_mutex_destroy (&c->log->mutex);
   if (err != 0)
      ERRNO_SET_RETURN (false, err, "pthread mutex destroy log");

   err = pthread_mutex_destroy (&c->event->mutex);
   if (err != 0)
      ERRNO_SET_RETURN (false, err, "pthread mutex destroy app");

   err = pthread_cond_destroy (&c->log->cond);
   if (err != 0)
      ERRNO_SET_RETURN (false, err, "pthread cond destroy log");

   err = pthread_cond_destroy (&c->event->cond);
   if (err != 0)
      ERRNO_SET_RETURN (false, err, "pthread cond destroy app");

   return true;
}

ThreadIdx
get_thread_idx (pthread_t thread_id)
{
   for (size_t i = 0; i < THREAD_COUNT; ++i)
      if (pthread_equal (s_thread_ids[i], thread_id))
         return (ThreadIdx)i;

   return -1;
}

const char*
thread_id_to_name (pthread_t thread_id)
{
   ThreadIdx idx = get_thread_idx (thread_id);

   return thread_idx_to_name (idx);
}

const char*
thread_idx_to_name (ThreadIdx thread_idx)
{
   if (thread_idx >= 0 && thread_idx < THREAD_COUNT)
      return thread_names[thread_idx];

   return "unknown";
}
