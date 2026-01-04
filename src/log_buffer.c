#include "log_buffer.h"

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LogBuffer*
log_buffer_create (const size_t size)
{
   LogBuffer* lb = malloc (sizeof (LogBuffer));
   if (lb == NULL)
   {
      perror ("lb malloc");
      return NULL;
   }

   lb->data = calloc (LOG_BUFFER_SIZE, sizeof (char));
   if (lb->data == NULL)
   {
      perror ("data calloc");
      return NULL;
   }

   lb->pos      = 0;
   lb->count    = 0;
   lb->capacity = size;

   return lb;
}

void
log_buffer_write_char (LogBuffer* lb, const char ch)
{
   lb->data[lb->pos] = ch;

   lb->pos = (lb->pos + 1) % lb->capacity;

   if (lb->count < lb->capacity)
      lb->count += 1;
}

void
log_buffer_write_string (LogBuffer* lb, const char* str)
{
   size_t str_len = strlen (str);

   if (str_len > LOG_BUFFER_SIZE - 1)
      str_len = LOG_BUFFER_SIZE - 1;

   char* start_ptr = &lb->data[lb->pos];

   if (lb->pos + str_len < lb->capacity)
      memcpy (start_ptr, str, str_len);
   else
   {
      const size_t top_part_size = lb->capacity - lb->pos;

      memcpy (start_ptr, str, top_part_size);
      memcpy (lb->data, &str[top_part_size], str_len - top_part_size);
   }

   lb->pos = (lb->pos + str_len) % lb->capacity;

   // keeping \0 at the pos, which will be overwritten by the first char
   // of the next string
   lb->data[lb->pos] = '\0';

   if (lb->count < lb->capacity)
      lb->count = lb->count + str_len > lb->capacity ? lb->capacity
                                                     : lb->count + str_len;
}

LogView
log_buffer_view (LogBuffer* lb)
{
   if (lb->count < lb->capacity)
      return (LogView){ .top = { .data = lb->data, .length = lb->count } };

   size_t first_char_pos = lb->count == lb->capacity ? lb->pos + 1 : 0;
   return (LogView){
      .top    = { .data   = &lb->data[first_char_pos],
                  .length = lb->capacity - first_char_pos, },
      .bottom = { .data   = lb->data,
                  .length = first_char_pos - 1, },
   };
}

void
log_buffer_print (LogBuffer* lb)
{
   if (lb->count == 0)
      return;

   LogView lv = log_buffer_view (lb);

   fwrite (lv.top.data, sizeof (char), lv.top.length, stderr);
   fwrite (lv.bottom.data, sizeof (char), lv.bottom.length, stderr);
}

char*
log_buffer_copy (LogBuffer* lb)
{
   char* log = malloc (lb->count * sizeof (char));
   if (log == NULL)
   {
      perror ("malloc");
      return NULL;
   }

   size_t first_char_pos = lb->count == lb->capacity ? lb->pos + 1 : 0;

   // TODO: rewrite using double memcpy
   for (size_t i = 0; i < lb->count; ++i)
      log[i] = lb->data[(i + first_char_pos) % lb->capacity];

   return log;
}

void
log_buffer_clear (LogBuffer* lb)
{
   lb->pos   = 0;
   lb->count = 0;
}

void
log_buffer_deinit (LogBuffer* lb)
{
   if (lb == NULL)
      return;

   if (lb->data == NULL)
      goto free_lb;

   free (lb->data);

free_lb:
   free (lb);
}
