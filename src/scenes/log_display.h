#pragma once

#include <raylib.h>

#include "../context.h"

void log_display_init(Context* c);
void log_display_update(Context* c, float dt);
void log_display_draw(Context* c, Image* buf);
void log_display_deinit();
