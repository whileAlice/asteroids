#pragma once

#include "../context.h"

#include <raylib.h>

// clang-format off
bool vector_products_scene_init   (Context* c);
bool vector_products_scene_deinit (void);
void vector_products_scene_update (Context* c, float dt);
void vector_products_scene_draw   (Context* c, Image* buf);
