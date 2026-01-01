#pragma once

#include "../context.h"

#include <raylib.h>

void vector_products_scene_init (Context* c);
void vector_products_scene_deinit ();
void vector_products_scene_update (Context* c, float dt);
void vector_products_scene_draw (Context* c, Image* buf);
