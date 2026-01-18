#pragma once

#include "../context.h"

#include <raylib.h>

// clang-format off
bool main_menu_bg_scene_init   (Context* c);
bool main_menu_bg_scene_deinit (void);
void main_menu_bg_scene_update (Context* c, float dt);
void main_menu_bg_scene_draw   (Context* c);
