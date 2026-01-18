#pragma once

#include "error.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct string_view {
   const char* const data;
   const size_t      length;
} StringView;

typedef struct string_builder {
   char*  data;
   size_t size;
   size_t capacity;
} StringBuilder;

// clang-format off
StringBuilder* sb_create   (const char* initial_string);
StringBuilder* sb_vcreatef (const char* initial_fmt, va_list args);
StringBuilder* sb_createf  (const char* initial_fmt, ...);
StringView     sb_view     (StringBuilder* sb);
bool           sb_append   (StringBuilder* sb, const char* string);
bool           sb_vappendf (StringBuilder* sb, const char* fmt, va_list args);
bool           sb_appendf  (StringBuilder* sb, const char* fmt, ...);
void           sb_clear    (StringBuilder* sb);
void           sb_free     (StringBuilder* sb);
// clang-format on

// duplicate a string with formatting - va_list version
static inline char*
vstrdupf (const char* fmt, va_list args)
{
   char*   dup;
   size_t  dup_size;
   va_list src_args_copy;
   va_copy (src_args_copy, args);

   int length = vsnprintf (NULL, 0, fmt, args);
   if (length == -1)
      ERRNO_RETURN ("null vsnprintf", NULL);

   dup_size = length * sizeof (char) + 1;
   dup      = malloc (dup_size);
   if (dup == NULL)
      ERRNO_RETURN ("malloc", NULL);

   length = vsnprintf (dup, dup_size, fmt, src_args_copy);
   if (length == -1)
      ERRNO_RETURN ("dup vsnprintf", NULL);

   return dup;
}

// duplicate a string with formatting - variadic version
static inline char*
strdupf (const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   char* dup = vstrdupf (fmt, args);
   if (dup == NULL)
      ERROR_RETURN ("vstrdupf", NULL);

   return dup;
}

// get nearest power of two greater than or equal to the given number
static inline size_t
align_to_power_of_two (const size_t num)
{
   size_t power_of_two = 2;
   while (power_of_two < num)
      power_of_two <<= 1;

   return power_of_two;
}
