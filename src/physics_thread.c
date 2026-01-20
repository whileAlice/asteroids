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

   log_debug ("Returning...");

   ThreadIdx thread_idx = get_thread_idx (pthread_self ());
   if (has_error (thread_idx))
   {
      error_print (thread_idx);
      app_quit_initiate (c);
   }

   return NULL;
}
