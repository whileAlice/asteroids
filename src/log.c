#include "log.h"

#include "config.h"
#include "context.h"
#include "terminal.h"
#include "threads.h"

#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PRINT_GOTO(label, msg)  \
   do                           \
   {                            \
      fputs (msg "\n", stderr); \
      goto label;               \
   }                            \
   while (0)

#define PERROR_GOTO(label, msg) \
   do                           \
   {                            \
      perror (msg);             \
      goto label;               \
   }                            \
   while (0)

#define INFO_PREFIX    "INFO:"
#define WARNING_PREFIX "WARNING:"
#define ERROR_PREFIX   "ERROR:"
#define DEBUG_PREFIX   "DEBUG:"
#define RAYLIB_PREFIX  "[raylib]"

static const char* s_log_names[LOG_COUNT] = {
   [STOUT_LOG]    = "stdout",
   [STDERR_LOG]   = "stderr",
   [INTERNAL_LOG] = "internal",
};

Log*
log_create (void)
{
   Log* log = calloc (1, sizeof (Log));
   if (log == NULL)
      ERRNO_RETURN (NULL, "calloc");

   log->buffers = calloc (LOG_COUNT, sizeof (LogBuffer*));

   for (size_t i = 0; i < LOG_COUNT; ++i)
   {
      log->buffers[i] = log_buffer_create (LOG_BUFFER_SIZE);
      if (log->buffers[i] == NULL)
         ERROR_RETURN (NULL, "log buffer create %s", s_log_names[i]);
   }

   return log;
}

void
log_free (Log* log)
{
   if (log == NULL)
      return;

   for (size_t i = 0; i < LOG_COUNT; ++i)
      log_buffer_free (log->buffers[i]);

   free (log->buffers);

   free (log);
}

void
vlog_to_file (FILE* output, LogLevel log_level, const char* fmt, va_list args)
{
   va_list args_copy, args_fail;
   va_copy (args_copy, args);
   va_copy (args_fail, args);

   TermColor   prefix_color = TERM_DEFAULT;
   const char* prefix       = INFO_PREFIX;

   switch (log_level)
   {
      case INFO_LOG_LEVEL:
         prefix_color = TERM_BLUE;
         prefix       = INFO_PREFIX;
         break;
      case WARNING_LOG_LEVEL:
         prefix_color = TERM_YELLOW;
         prefix       = WARNING_PREFIX;
      case ERROR_LOG_LEVEL:
         prefix_color = TERM_RED;
         prefix       = ERROR_PREFIX;
         break;
      case DEBUG_LOG_LEVEL:
         prefix_color = TERM_MAGENTA;
         prefix       = DEBUG_PREFIX;
         break;
      default: assert (0 == "Unreachable");
   }

   char* thread_prefix = strdupf ("[%s]", thread_id_to_name (pthread_self ()));
   if (thread_prefix == NULL)
      PRINT_GOTO (fail, "strdupf");

   char* formatted_thread_prefix = get_term_formatted_string (
      TERM_GREEN, TERM_DEFAULT, false, thread_prefix);
   if (formatted_thread_prefix == NULL)
      PRINT_GOTO (fail, "get term formatted thread prefix");

   // TODO: assemble all these strings in a single temp buffer
   char* formatted_prefix =
      get_term_formatted_string (prefix_color, TERM_DEFAULT, true, prefix);
   if (formatted_prefix == NULL)
      PRINT_GOTO (fail, "get term formatted prefix");

   int length = vsnprintf (NULL, 0, fmt, args);
   if (length < 0)
      PERROR_GOTO (fail, "null vsnprintf");

   size_t message_size = length * sizeof (char) + 1;
   char*  message      = malloc (message_size);
   if (message == NULL)
      PERROR_GOTO (fail, "message malloc");

   length = vsnprintf (message, message_size, fmt, args_copy);
   if (length < 0)
      PERROR_GOTO (fail, "message vsnprintf");

   length = snprintf (NULL, 0, "%s %s %s\n", formatted_prefix,
                      formatted_thread_prefix, message);
   if (length < 0)
      PERROR_GOTO (fail, "null snprintf");

   size_t message_with_prefix_size = length * sizeof (char) + 1;
   char*  message_with_prefix      = malloc (message_with_prefix_size);
   if (message_with_prefix == NULL)
      PERROR_GOTO (fail, "message with prefix malloc");

   length =
      snprintf (message_with_prefix, message_with_prefix_size, "%s %s %s\n",
                formatted_prefix, formatted_thread_prefix, message);
   if (length < 0)
      PERROR_GOTO (fail, "message with prefix snprintf");

   fputs (message_with_prefix, output);

   free (message);
   free (thread_prefix);
   free (formatted_prefix);
   free (message_with_prefix);
   free (formatted_thread_prefix);

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

   // TODO: assemble all these strings in a single temp buffer and preferably
   // integrate it with the assembly in log_to_file
   char* formatted_prefix =
      get_term_formatted_string (TERM_CYAN, TERM_DEFAULT, false, RAYLIB_PREFIX);
   if (formatted_prefix == NULL)
      PRINT_GOTO (fail, "get term formatted string");

   int length = vsnprintf (NULL, 0, fmt, args);
   // TODO: maybe these shouldn't be as immediately fatal as in our logs?
   if (length < 0)
      PERROR_GOTO (fail, "null vsnprintf");

   size_t message_size = length * sizeof (char) + 1;
   char*  message      = malloc (message_size);
   if (message == NULL)
      PERROR_GOTO (fail, "message malloc");

   length = vsnprintf (message, message_size, fmt, args_copy);
   if (length < 0)
      PERROR_GOTO (fail, "message vsnprintf");

   length = snprintf (NULL, 0, "%s %s", formatted_prefix, message);
   if (length < 0)
      PERROR_GOTO (fail, "null snprintf");

   size_t message_with_prefix_size = length * sizeof (char) + 1;
   char*  message_with_prefix      = malloc (message_with_prefix_size);
   if (message_with_prefix == NULL)
      PERROR_GOTO (fail, "message with prefix malloc");

   length = snprintf (message_with_prefix, message_with_prefix_size, "%s %s",
                      formatted_prefix, message);
   if (length < 0)
      PERROR_GOTO (fail, "message with prefix snprintf");

   switch (log_level)
   {
      case LOG_TRACE:
      case LOG_DEBUG:
         log_to_file (stderr, DEBUG_LOG_LEVEL, message_with_prefix);
         break;
      case LOG_INFO:
         log_to_file (stderr, INFO_LOG_LEVEL, message_with_prefix);
         break;
      case LOG_WARNING:
         log_to_file (stderr, WARNING_LOG_LEVEL, message_with_prefix);
         break;
      case LOG_ERROR:
      case LOG_FATAL:
         log_to_file (stderr, ERROR_LOG_LEVEL, message_with_prefix);
         break;
      default: assert (0 == "Unreachable");
   }

   free (message);
   free (formatted_prefix);
   free (message_with_prefix);

   return;

fail:
   fputs ("Fatal failure in raylib_tracelog_callback(). Last error message: ",
          stderr);
   vfprintf (stderr, fmt, args_fail);
   fputc ('\n', stderr);

   abort ();
}
