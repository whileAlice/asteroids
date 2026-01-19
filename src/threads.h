#pragma once

#include <pthread.h>

// clang-format off
#define IN_LOCK(mutex_ptr, statements)  \
   do                                   \
   {                                    \
      pthread_mutex_lock (mutex_ptr);   \
      statements                        \
      pthread_mutex_unlock (mutex_ptr); \
   }                                    \
   while (0)
// clang-format on

#define SYNC_TWO_THREADS(mutex_ptr, cond_ptr, ready_count, escape_condition, \
                         escape_label)                                       \
   do                                                                        \
   {                                                                         \
      pthread_mutex_lock (mutex_ptr);                                        \
                                                                             \
      ready_count += 1;                                                      \
      pthread_cond_signal (cond_ptr);                                        \
                                                                             \
      while (ready_count < 2 && !escape_condition)                           \
         pthread_cond_wait (cond_ptr, mutex_ptr);                            \
                                                                             \
      if (escape_condition)                                                  \
      {                                                                      \
         pthread_mutex_unlock (mutex_ptr);                                   \
         goto escape_label;                                                  \
      }                                                                      \
                                                                             \
      pthread_mutex_unlock (mutex_ptr);                                      \
   }                                                                         \
   while (0);

typedef struct context Context;

typedef enum thread_idx {
   MAIN_THREAD = 0,
   PHYSICS_THREAD,
   THREAD_COUNT,
} ThreadIdx;

#define UNKNOWN_THREAD THREAD_COUNT

// clang-format off
bool        threads_init       (Context* c);
bool        threads_deinit     (Context* c);
ThreadIdx   get_thread_idx     (pthread_t thread_id);
const char* thread_id_to_name  (pthread_t thread_id);
const char* thread_idx_to_name (ThreadIdx thread_idx);
