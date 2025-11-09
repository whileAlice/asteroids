#pragma once

#include <stddef.h>
#include <raylib.h>

#include "context.h"

void init_log_buffers(Context* c);
void clear_osd_buffer(Context* c);
void osd_print(size_t row, size_t col, const char* text);
void osd_printf(size_t row, size_t col, const char* fmt, ...);
void log_print(const char* line);
void log_printf(const char* fmt, ...);
void draw_log(Context* c, Image* buf, bool is_inverted);
void draw_osd(Context* c, Image* buf, bool is_inverted);
void previous_log_page(Context* c);
void next_log_page(Context* c);
void deinit_log_buffers();
