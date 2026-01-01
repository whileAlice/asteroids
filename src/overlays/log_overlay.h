#pragma once

#include "../context.h"
#include "../ui_layer.h"

#include <raylib.h>

void log_overlay_init (UILayer* self, Context* c);
void log_overlay_deinit (UILayer* self);
void log_overlay_update (UILayer* self, Context* c, float dt);
void log_overlay_draw (UILayer* self, Context* c, Image* buf);
