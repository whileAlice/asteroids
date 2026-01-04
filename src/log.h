#pragma once

#include "context.h"

// clang-format off
Log* log_create (void);
void log_deinit (Log* log);
void msg        (const char* str);
