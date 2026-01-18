#pragma once

#include <pthread.h>

#define ERROR_RETURN(retval, fmt, ...) \
   do                                  \
   {                                   \
      error_set (fmt, ##__VA_ARGS__);  \
      return retval;                   \
   }                                   \
   while (0)

#define ERRNO_RETURN(retval, fmt, ...)           \
   do                                            \
   {                                             \
      error_set_with_errno (fmt, ##__VA_ARGS__); \
      return retval;                             \
   }                                             \
   while (0)

#define ERROR_GOTO(label, fmt, ...)   \
   do                                 \
   {                                  \
      error_set (fmt, ##__VA_ARGS__); \
      goto label;                     \
   }                                  \
   while (0)

#define ERRNO_GOTO(label, fmt, ...)              \
   do                                            \
   {                                             \
      error_set_with_errno (fmt, ##__VA_ARGS__); \
      goto label;                                \
   }                                             \
   while (0)

#define ERRNO_FORMAT "errno %d: '%s'"

// NOTE: errors are fatal, there can be only one per thread
typedef struct error {
   char*         message;
   struct error* next;
} Error;

// clang-format off
void error_set            (const char* fmt, ...);
void error_set_with_errno (const char* fmt, ...);
void error_print          (Error* error);
void errors_print         (void);
void errors_free          (void);
