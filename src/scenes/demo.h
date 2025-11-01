#pragma once

#include <raylib.h>

#include "../context.h"

void demo_init(Context* c);
void demo_update(Context* c, float dt);
void demo_draw(Context* c, Image* buf);
void demo_deinit();
