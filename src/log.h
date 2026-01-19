#pragma once

#include "context.h"

#include <stdarg.h>

#define WARNING_RETURN(retval, fmt, ...) \
   do                                    \
   {                                     \
      log_warning (fmt, ##__VA_ARGS__);  \
      return retval;                     \
   }                                     \
   while (0)

typedef enum log_idx {
   STDOUT_LOG = 0,
   STDERR_LOG,
   INTERNAL_LOG,
   LOG_COUNT,
} LogIdx;

typedef enum log_level {
   INFO_LOG_LEVEL,
   WARNING_LOG_LEVEL,
   ERROR_LOG_LEVEL,
   DEBUG_LOG_LEVEL,
   LOG_LEVEL_COUNT,
} LogLevel;

// clang-format off
Log* log_create               (void);
void log_free                 (Log* log);
void vlog_to_file             (FILE* output, LogLevel log_level, const char* fmt, va_list args);
void log_to_file              (FILE* output, LogLevel log_level, const char* fmt, ...);
void log_info                 (const char* fmt, ...);
void log_warning              (const char* fmt, ...);
void log_error                (const char* fmt, ...);
void log_debug                (const char* fmt, ...);
void raylib_tracelog_callback (int log_level, const char *text, va_list args);
