#pragma once

#include <raylib.h>

#include "../context.h"

void demo_scene_init  (Context* c);
void demo_scene_deinit();
void demo_scene_update(Context* c, float dt);
void demo_scene_draw  (Context* c, Image* buf);
