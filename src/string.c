#include "string.h"

#include "error.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SB_CAPACITY 32

StringBuilder*
sb_create (const char* initial_string)
{
   // empty StringBuilder starts with a null byte
   size_t size     = 1;
   size_t capacity = INITIAL_SB_CAPACITY;
   if (initial_string != NULL)
   {
      size     = strlen (initial_string) + 1;
      capacity = align_to_power_of_two (size);
   }

   StringBuilder* sb = malloc (sizeof (StringBuilder));
   if (sb == NULL)
      ERRNO_RETURN (NULL, "sb malloc");

   char* data = malloc (capacity * sizeof (char));
   if (data == NULL)
      ERRNO_RETURN (NULL, "data calloc");

   if (initial_string != NULL)
      strcpy (data, initial_string);
   else
      data[0] = '\0';

   *sb = (StringBuilder){
      .data     = data,
      .size     = size,
      .capacity = capacity,
   };

   return sb;
}

StringBuilder*
sb_vcreatef (const char* initial_fmt, va_list args)
{
   char* str = vstrdupf (initial_fmt, args);
   if (str == NULL)
      ERROR_RETURN (NULL, "vstrdupf");

   StringBuilder* sb = sb_create (str);
   if (sb == NULL)
      ERROR_RETURN (NULL, "sb create");

   free (str);

   return sb;
}

StringBuilder*
sb_createf (const char* initial_fmt, ...)
{
   va_list args;
   va_start (args, initial_fmt);

   StringBuilder* sb = sb_vcreatef (initial_fmt, args);
   if (sb == NULL)
      ERROR_RETURN (NULL, "sb vcreatef");

   va_end (args);

   return sb;
}

StringView
sb_view (StringBuilder* sb)
{
   return (StringView){
      .data   = sb->data,
      .length = sb->size - 1,
   };
}

bool
sb_append (StringBuilder* sb, const char* string)
{
   assert (string != NULL);

   size_t len = strlen (string);

   if (len + sb->size > sb->capacity)
   {
      size_t new_capacity = sb->capacity * 2;

      if (new_capacity < len + sb->size)
         new_capacity = align_to_power_of_two (len + sb->size);

      sb->data = realloc (sb->data, new_capacity * sizeof (char));
      if (sb->data == NULL)
         ERRNO_RETURN (false, "realloc");

      sb->capacity = new_capacity;
   }

   char* start_ptr = &sb->data[sb->size - 1];
   strcpy (start_ptr, string);

   sb->size += len;

   return true;
}

bool
sb_vappendf (StringBuilder* sb, const char* fmt, va_list args)
{
   char* str = vstrdupf (fmt, args);
   if (str == NULL)
      ERROR_RETURN (NULL, "vstrdupf");

   bool success = sb_append (sb, str);
   if (!success)
      ERROR_RETURN (false, "sb append");

   free (str);

   return true;
}

bool
sb_appendf (StringBuilder* sb, const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   bool success = sb_vappendf (sb, fmt, args);
   if (!success)
      ERROR_RETURN (false, "sb vappendf");

   va_end (args);

   return true;
}

void
sb_clear (StringBuilder* sb)
{
   assert (sb != NULL);

   sb->size    = 1;
   sb->data[0] = '\0';
}

void
sb_free (StringBuilder* sb)
{
   if (sb == NULL)
      return;

   free (sb->data);
   free (sb);
}
