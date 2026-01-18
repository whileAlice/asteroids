#pragma once

#include "../context.h"

#include <raylib.h>

// clang-format off
bool osd_overlay_init   (Context* c);
bool osd_overlay_deinit (Context* c);
void osd_overlay_update (Context* c, float dt);
void osd_overlay_draw   (Context* c);
