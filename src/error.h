#pragma once

#include "context.h"

#include <pthread.h>

// NOTE: errors are fatal, there can be only one per thread
typedef struct error {
   char*         message;
   struct error* next;
} Error;

// clang-format off
void errors_init   (App* app);
void errors_deinit (void);
void set_error     (const char* fmt, ...);
