#pragma once

#include <raylib.h>

#include "../context.h"

void main_menu_layer_init  (Context* c);
void main_menu_layer_deinit();
void main_menu_layer_update(Context* c, float dt);
void main_menu_layer_draw  (Context* c, Image* buf);
