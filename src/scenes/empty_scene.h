#pragma once

#include "../context.h"

#include <raylib.h>

// clang-format off
bool empty_scene_init   (Context* c);
bool empty_scene_deinit (Context* c);
void empty_scene_update (Context* c, float dt);
void empty_scene_draw   (Context* c);
