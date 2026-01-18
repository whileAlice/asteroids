#pragma once

#include "../context.h"
#include "../ui_layer.h"

#include <raylib.h>

// clang-format off
bool main_menu_modal_init   (UILayer* self, Context* c);
bool main_menu_modal_deinit (UILayer* self);
void main_menu_modal_update (UILayer* self, Context* c, float dt);
void main_menu_modal_draw   (UILayer* self, Context* c, Image* buf);
