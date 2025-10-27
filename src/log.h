#pragma once

#include <raylib.h>
#include <stddef.h>

void init_log_buffers();
void osd_print(size_t row, size_t col, const char* text);
void osd_printf(size_t row, size_t col, const char* fmt, ...);
void add_line_to_console_log(const char* line);
void draw_console_log(Image* buf);
void draw_osd(Image* buf);
void deinit_log_buffers();
