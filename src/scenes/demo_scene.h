#pragma once

#include "../context.h"

#include <raylib.h>

// clang-format off
bool demo_scene_init   (Context* c);
bool demo_scene_deinit (void);
void demo_scene_update (Context* c, float dt);
void demo_scene_draw   (Context* c, Image* buf);
