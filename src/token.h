#pragma once

#include <stdio.h>

char* read_next_line(FILE* fp, char* buf, const char* filename,
                     int* line_count);
