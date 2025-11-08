#pragma once

#include <raylib.h>

#include "../context.h"

void menu_init(Context* c);
void menu_update(Context* c, float dt);
void menu_draw(Context* c, Image* buf);
void menu_deinit();
