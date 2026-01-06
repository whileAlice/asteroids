#pragma once

#include "context.h"

#include <stdarg.h>

typedef enum log_idx {
   STOUT_LOG = 0,
   STDERR_LOG,
   INTERNAL_LOG,
   LOG_COUNT,
} LogIdx;

typedef enum log_level {
   DEBUG_LOG_LEVEL,
   INFO_LOG_LEVEL,
   ERROR_LOG_LEVEL,
   LOG_LEVEL_COUNT,
} LogLevel;

// clang-format off
Log* log_create               (void);
void log_deinit               (Log* log);
void vlog_to_file             (FILE* output, LogLevel log_level, bool with_newline, const char* fmt, va_list args);
void log_to_file              (FILE* output, LogLevel log_level, bool with_newline, const char* fmt, ...);
void info                     (const char* fmt, ...);
void debug                    (const char* fmt, ...);
void error                    (const char* fmt, ...);
void raylib_tracelog_callback (int log_level, const char *text, va_list args);
