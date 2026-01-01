#pragma once

#include "../context.h"
#include "../ui_layer.h"

#include <raylib.h>

void osd_overlay_init (UILayer* self, Context* c);
void osd_overlay_deinit (UILayer* self);
void osd_overlay_update (UILayer* self, Context* c, float dt);
void osd_overlay_draw (UILayer* self, Context* c, Image* buf);
