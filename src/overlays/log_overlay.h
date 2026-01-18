#pragma once

#include "../context.h"

#include <raylib.h>

// clang-format off
bool log_overlay_init   (Context* c);
bool log_overlay_deinit (void);
void log_overlay_update (Context* c, float dt);
void log_overlay_draw   (Context* c);
