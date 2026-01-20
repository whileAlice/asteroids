#pragma once

#include "string.h"

#include <stddef.h>

// NOTE: top is null-terminated only if bottom does not exist.
// If bottom exists, it is null-terminated.
typedef struct log_view {
   const StringView top;
   const StringView bottom;
} LogView;

typedef struct log_buffer {
   char*  data;
   size_t pos;
   size_t size;
   size_t capacity;
} LogBuffer;

// clang-format off
LogBuffer* log_buffer_create (const size_t size);
void log_buffer_write_char (LogBuffer* lb, const char  ch);
void log_buffer_write_string (LogBuffer* lb, const char* str);
LogView log_buffer_view (LogBuffer* lb);
void log_buffer_print (LogBuffer* lb);
char* log_buffer_copy (LogBuffer* lb);
void log_buffer_clear (LogBuffer* lb);
void log_buffer_free (LogBuffer* lb);
