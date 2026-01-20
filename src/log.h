#pragma once

#include "log_buffer.h"

#include <stdarg.h>
#include <stdio.h>

#define WARNING_RETURN(retval, fmt, ...) \
   do                                    \
   {                                     \
      log_warning (fmt, ##__VA_ARGS__);  \
      return retval;                     \
   }                                     \
   while (0)

typedef enum log_idx {
   RAYLIB_LOG = 0,
   INTERNAL_LOG,
   LOG_COUNT,
} LogIdx;

typedef enum log_level {
   INFO_LOG_LEVEL = 0,
   WARNING_LOG_LEVEL,
   ERROR_LOG_LEVEL,
   DEBUG_LOG_LEVEL,
   RAYLIB_LOG_LEVEL,
   LOG_LEVEL_COUNT,
} LogLevel;

// clang-format off
bool log_init (void);
void log_deinit (void);
char* get_log_copy (LogIdx log_idx);
void vlog_to_file (FILE* output, LogLevel log_level, const char* fmt, va_list args);
void log_to_file (FILE* output, LogLevel log_level, const char* fmt, ...);
void log_to_buffer (LogBuffer* lb, LogLevel log_level, const char* str);
void log_info (const char* fmt, ...);
void log_warning (const char* fmt, ...);
void log_error (const char* fmt, ...);
void log_debug (const char* fmt, ...);
void raylib_tracelog_callback (int log_level, const char *text, va_list args);
