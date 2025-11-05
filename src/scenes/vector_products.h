#pragma once

#include <raylib.h>

#include "../context.h"

void vector_products_init(Context* c);
void vector_products_update(Context* c, float dt);
void vector_products_draw(Context* c, Image* buf);
void vector_products_deinit();
