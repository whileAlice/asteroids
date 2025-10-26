#pragma once

#include <raylib.h>

#include "font.h"

void init_log_buffers(FixedFont* font);
void add_line_to_console_log(const char* line);
void show_console_log(Image* buf);
void print_to_osd(const char* text, size_t row, size_t col);
void show_osd(Image* buf);
void deinit_log_buffers();
