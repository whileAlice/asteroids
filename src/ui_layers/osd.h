#pragma once

#include <raylib.h>

#include "../context.h"

void osd_layer_init  (Context* c);
void osd_layer_deinit();
void osd_layer_update(Context* c, float dt);
void osd_layer_draw  (Context* c, Image* buf);
