#pragma once

#include <stddef.h>

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
StringBuilder* sb_create (const char* initial_string);
StringView     sb_view   (StringBuilder* sb);
bool           sb_append (StringBuilder* sb, const char* string);
void           sb_clear  (StringBuilder* sb);
void           sb_deinit (StringBuilder* sb);

// get nearest power of two greater than or equal to the given number
static inline size_t align_to_power_of_two (const size_t num);
