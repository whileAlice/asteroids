#include "log.h"

#include "config.h"
#include "context.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Log*
log_create (void)
{
   Log* log = calloc (1, sizeof (Log));
   if (log == NULL)
   {
      perror ("log calloc");
      return NULL;
   }

   log->buffer = log_buffer_create (LOG_BUFFER_SIZE);
   if (log->buffer == NULL)
   {
      perror ("log buffer create");
      return NULL;
   }

   return log;
}

void
msg (const char* str)
{
   printf ("%s", str);
}

void
log_deinit (Log* log)
{
   log_buffer_deinit (log->buffer);
   free (log);
}
