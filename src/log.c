#include "log.h"

#include "config.h"
#include "raylib.h"
#include "terminal.h"
#include "threads.h"

#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STDERR_PRINT_GOTO(label, msg) \
   do                                 \
   {                                  \
      fputs (msg "\n", stderr);       \
      goto label;                     \
   }                                  \
   while (0)

#define INFO_PREFIX    "INFO:"
#define WARNING_PREFIX "WARN:"
#define ERROR_PREFIX   "!ERR:"
#define DEBUG_PREFIX   "~DBG:"
#define RAYLIB_PREFIX  "RLIB:"

static LogBuffer*  s_logs[LOG_COUNT];
static const char* s_log_names[LOG_COUNT] = {
   [RAYLIB_LOG]   = "raylib",
   [INTERNAL_LOG] = "internal",
};

bool
log_init (void)
{
   for (size_t i = 0; i < LOG_COUNT; ++i)
   {
      s_logs[i] = log_buffer_create (LOG_BUFFER_SIZE);
      if (s_logs[i] == NULL)
         ERROR_RETURN (false, "log buffer create %s", s_log_names[i]);
   }
   return true;
}

void
log_deinit (void)
{
   for (size_t i = 0; i < LOG_COUNT; ++i)
   {
      log_buffer_free (s_logs[i]);
      s_logs[i] = NULL;
   }
}



char*
get_log_copy (LogIdx log_idx)
{
   assert (log_idx >= 0);
   assert (log_idx < LOG_COUNT);

   LogBuffer* lb  = s_logs[log_idx];
   char*      log = log_buffer_copy (lb);

   return log;
}

void
vlog_to_file (FILE* output, LogLevel log_level, const char* fmt, va_list args)
{
   va_list args_copy, args_fail;
   va_copy (args_copy, args);
   va_copy (args_fail, args);

   char*       term_message;
   TermColor   prefix_color;
   const char* prefix;

   switch (log_level)
   {
      case INFO_LOG_LEVEL:
         prefix_color = TERM_BLUE;
         prefix       = INFO_PREFIX;
         break;
      case WARNING_LOG_LEVEL:
         prefix_color = TERM_YELLOW;
         prefix       = WARNING_PREFIX;
         break;
      case ERROR_LOG_LEVEL:
         prefix_color = TERM_RED;
         prefix       = ERROR_PREFIX;
         break;
      case DEBUG_LOG_LEVEL:
         prefix_color = TERM_MAGENTA;
         prefix       = DEBUG_PREFIX;
         break;
      case RAYLIB_LOG_LEVEL:
         prefix_color = TERM_GREEN;
         prefix       = RAYLIB_PREFIX;
         break;
      default: assert (0 == "Unreachable");
   }

   char* formatted_prefix =
      get_term_formatted_string (prefix_color, TERM_DEFAULT, true, prefix);
   if (formatted_prefix == NULL)
      STDERR_PRINT_GOTO (fail, "get term formatted prefix");

   char* message = vstrdupf (fmt, args);
   if (message == NULL)
      STDERR_PRINT_GOTO (fail, "message vstrdupf");

   if (log_level != RAYLIB_LOG_LEVEL)
   {
      const char* thread_name = thread_id_to_name (pthread_self ());

      char* thread_prefix = strdupf ("[%s]", thread_name);
      if (thread_prefix == NULL)
         STDERR_PRINT_GOTO (fail, "thread prefix strdupf");

      char* formatted_thread_prefix = get_term_formatted_string (
         TERM_GREEN, TERM_DEFAULT, false, thread_prefix);
      if (formatted_thread_prefix == NULL)
         STDERR_PRINT_GOTO (fail, "get term formatted thread prefix");

      char* log_buffer_message =
         strdupf ("%s %s %s\n", prefix, thread_prefix, message);
      if (log_buffer_message == NULL)
         STDERR_PRINT_GOTO (fail, "buffer message with prefix strdupf");

      log_buffer_write_string (s_logs[INTERNAL_LOG], log_buffer_message);

      term_message = strdupf ("%s %s %s\n", formatted_prefix,
                              formatted_thread_prefix, message);
      if (term_message == NULL)
         STDERR_PRINT_GOTO (fail, "term message strdupf");

      free (thread_prefix);
      free (log_buffer_message);
      free (formatted_thread_prefix);
   }
   else
   {
      log_buffer_write_string (s_logs[RAYLIB_LOG], message);
      log_buffer_write_char (s_logs[RAYLIB_LOG], '\n');

      term_message = strdupf ("%s %s\n", formatted_prefix, message);
      if (term_message == NULL)
         STDERR_PRINT_GOTO (fail, "term message strdupf");
   }

   fputs (term_message, output);

   free (message);
   free (term_message);
   free (formatted_prefix);

   return;

fail:
   fputs ("Fatal failure in vlog_to_file(). Last error message: ", stderr);
   vfprintf (stderr, fmt, args_fail);
   fputc ('\n', stderr);

   abort ();
}

void
log_to_file (FILE* output, LogLevel log_level, const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (output, log_level, fmt, args);

   va_end (args);
}

void
log_info (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (stderr, INFO_LOG_LEVEL, fmt, args);

   va_end (args);
}

void
log_warning (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (stderr, WARNING_LOG_LEVEL, fmt, args);

   va_end (args);
}

void
log_error (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (stderr, ERROR_LOG_LEVEL, fmt, args);

   va_end (args);
}

void
log_debug (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (stderr, DEBUG_LOG_LEVEL, fmt, args);

   va_end (args);
}

void
raylib_tracelog_callback (int log_level, const char* fmt, va_list args)
{
   va_list args_copy, args_fail;
   va_copy (args_copy, args);
   va_copy (args_fail, args);

   char* message = vstrdupf (fmt, args);
   if (message == NULL)
      STDERR_PRINT_GOTO (fail, "message vstrdupf");

   switch (log_level)
   {
      case LOG_TRACE:
         log_to_file (stderr, RAYLIB_LOG_LEVEL, "%s %s", "TRACE:", message);
         break;
      case LOG_DEBUG:
         log_to_file (stderr, RAYLIB_LOG_LEVEL, "%s %s", "DEBUG:", message);
         break;
      case LOG_INFO:
         log_to_file (stderr, RAYLIB_LOG_LEVEL, "%s %s", "INFO:", message);
         break;
      case LOG_WARNING:
         log_to_file (stderr, RAYLIB_LOG_LEVEL, "%s %s", "WARNING:", message);
         break;
      case LOG_ERROR:
         log_to_file (stderr, RAYLIB_LOG_LEVEL, "%s %s", "ERROR:", message);
         break;
      case LOG_FATAL:
         log_to_file (stderr, RAYLIB_LOG_LEVEL, "%s %s", "FATAL:", message);
         break;
      default: assert (0 == "Unreachable");
   }

   free (message);

   return;

fail:
   fputs ("Fatal failure in raylib_tracelog_callback(). Last error message: ",
          stderr);
   vfprintf (stderr, fmt, args_fail);
   fputc ('\n', stderr);

   abort ();
}
