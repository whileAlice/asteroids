#include "physics_thread.h"

#include "context.h"
#include "error.h"
#include "event.h"
#include "log.h"
#include "threads.h"

void*
physics_thread (void* arg)
{
   Context* c = (Context*)arg;
   Log*     l = c->log;

   log_debug ("Returning...");

   ThreadIdx thread_idx = get_thread_idx (pthread_self ());
   if (has_error (thread_idx))
   {
      error_print (thread_idx);

      IN_LOCK (&l->mutex,
         l->should_abort_init = true;
         pthread_cond_signal (&l->cond);
      );
      app_quit_initiate (c);
   }

   return NULL;
}
