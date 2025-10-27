#pragma once

#include <raylib.h>

void init_log_buffers();
void add_line_to_console_log(const char* line);
void draw_console_log(Image* buf);
void print_to_osd(const char* text, size_t row, size_t col);
void draw_osd(Image* buf);
void deinit_log_buffers();
