#include "log.h"

#include "config.h"
#include "context.h"
#include "terminal.h"

#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INFO_PREFIX  "INFO:"
#define DEBUG_PREFIX "DEBUG:"
#define ERROR_PREFIX "ERROR:"

#define RAYLIB_PREFIX "[raylib]"

pthread_mutex_t s_stream_mutex = PTHREAD_MUTEX_INITIALIZER;

Log*
log_create (void)
{
   Log* log = calloc (1, sizeof (Log));
   if (log == NULL)
   {
      perror ("log calloc");
      return NULL;
   }

   log->buffers = calloc (LOG_COUNT, sizeof (LogBuffer*));

   // TODO: use this inside error message after moving from perror
   const char* log_names[LOG_COUNT] = {
      [STOUT_LOG]    = "stdout",
      [STDERR_LOG]   = "stderr",
      [INTERNAL_LOG] = "internal",
   };
   for (size_t i = 0; i < LOG_COUNT; ++i)
   {
      log->buffers[i] = log_buffer_create (LOG_BUFFER_SIZE);
      if (log->buffers[i] == NULL)
      {
         perror ("log buffer create");
         return NULL;
      }
   }

   return log;
}

void
log_deinit (Log* log)
{
   for (size_t i = 0; i < LOG_COUNT; ++i)
      log_buffer_deinit (log->buffers[i]);

   free (log);
}

void
vlog_to_file (FILE* output, LogLevel log_level, bool with_newline,
              const char* fmt, va_list args)
{
   TermColor   prefix_color = TERM_DEFAULT;
   const char* prefix       = INFO_PREFIX;

   switch (log_level)
   {
   case DEBUG_LOG_LEVEL:
      prefix_color = TERM_MAGENTA;
      prefix       = DEBUG_PREFIX;
      break;
   case INFO_LOG_LEVEL:
      prefix_color = TERM_BLUE;
      prefix       = INFO_PREFIX;
      break;
   case ERROR_LOG_LEVEL:
      prefix_color = TERM_RED;
      prefix       = ERROR_PREFIX;
      break;
   default: assert (0 == "Unreachable");
   }

   // TODO: assemble all these strings in a single temp buffer
   char* formatted_prefix =
      get_term_formatted_string (prefix_color, TERM_DEFAULT, true, prefix);

   int length = vsnprintf (NULL, 0, fmt, args);
   // TODO: propagate
   if (length < 0)
   {
      perror ("NULL vsnprintf");
      abort ();
   }

   size_t message_size = length * sizeof (char) + 1;
   char*  message      = malloc (message_size);

   length = vsnprintf (message, message_size, fmt, args);
   if (length < 0)
   {
      perror ("message vsnprintf");
      abort ();
   }

   length = snprintf (NULL, 0, "%s %s", formatted_prefix, message);
   if (length < 0)
   {
      perror ("NULL snprintf");
      abort ();
   }

   size_t message_with_prefix_size = length * sizeof (char) + 1;
   char*  message_with_prefix =
      calloc (length + 1 + (with_newline ? 1 : 0), sizeof (char));

   length = snprintf (message_with_prefix, message_with_prefix_size, "%s %s",
                      formatted_prefix, message);
   if (length < 0)
   {
      perror ("message_with_prefix snprintf");
      abort ();
   }

   if (with_newline)
      message_with_prefix[length] = '\n';

   fputs (message_with_prefix, output);

   free (message);
   free (formatted_prefix);
   free (message_with_prefix);
}

void
log_to_file (FILE* output, LogLevel log_level, bool with_newline,
             const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (output, log_level, with_newline, fmt, args);

   va_end (args);
}

void
info (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (stderr, INFO_LOG_LEVEL, true, fmt, args);

   va_end (args);
}

void
debug (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (stderr, DEBUG_LOG_LEVEL, true, fmt, args);

   va_end (args);
}

void
error (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   vlog_to_file (stderr, ERROR_LOG_LEVEL, true, fmt, args);

   va_end (args);
}

void
raylib_tracelog_callback (int log_level, const char* fmt, va_list args)
{
   // TODO: assemble all these strings in a single temp buffer and preferably
   // integrate it with the assembly in log_to_file
   char* formatted_prefix = get_term_formatted_string (
      TERM_YELLOW, TERM_DEFAULT, false, RAYLIB_PREFIX);

   int length = vsnprintf (NULL, 0, fmt, args);
   // TODO: propagate
   if (length < 0)
   {
      perror ("NULL vsnprintf");
      abort ();
   }

   size_t message_size = length * sizeof (char) + 1;
   char*  message      = malloc (message_size);

   length = vsnprintf (message, message_size, fmt, args);
   if (length < 0)
   {
      perror ("message vsnprintf");
      abort ();
   }

   length = snprintf (NULL, 0, "%s %s", formatted_prefix, message);
   if (length < 0)
   {
      perror ("NULL snprintf");
      abort ();
   }

   size_t message_with_prefix_size = length * sizeof (char) + 1;
   char*  message_with_prefix      = malloc (message_with_prefix_size);

   length = snprintf (message_with_prefix, message_with_prefix_size, "%s %s",
                      formatted_prefix, message);
   if (length < 0)
   {
      perror ("message_with_prefix snprintf");
      abort ();
   }

   switch (log_level)
   {
   case LOG_TRACE:
   case LOG_DEBUG:
      log_to_file (stderr, DEBUG_LOG_LEVEL, true, message_with_prefix);
      break;
   case LOG_INFO:
      log_to_file (stderr, INFO_LOG_LEVEL, true, message_with_prefix);
      break;
   case LOG_WARNING:
   case LOG_ERROR:
   case LOG_FATAL:
      log_to_file (stderr, ERROR_LOG_LEVEL, true, message_with_prefix);
      break;
   default: assert (0 == "Unreachable");
   }

   free (message);
   free (formatted_prefix);
   free (message_with_prefix);
}
