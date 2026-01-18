#pragma once

#include <stdio.h>

// clang-format off
char* read_next_line (FILE* fp, char* buf, const char* filename, int* line_count);
