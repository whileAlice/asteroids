#pragma once

#include <raylib.h>
#include <stddef.h>

void init_log_buffers();
void osd_print(size_t row, size_t col, const char* text);
void osd_printf(size_t row, size_t col, const char* fmt, ...);
void log_print(const char* line);
void log_printf(const char* fmt, ...);
void draw_log(Image* buf, bool is_inverted);
void draw_osd(Image* buf, bool is_inverted);
void previous_log_page();
void next_log_page();
void deinit_log_buffers();
