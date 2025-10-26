#pragma once

#include <raylib.h>

#include "font.h"

void init_log_buffers(FixedFont* font);
void add_line_to_console_log(const char* line);
void show_console_log(Image* buf);
void deinit_log_buffers();
