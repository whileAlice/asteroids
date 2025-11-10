#pragma once

#include <raylib.h>

#include "../context.h"

void main_menu_scene_init  (Context* c);
void main_menu_scene_deinit();
void main_menu_scene_update(Context* c, float dt);
void main_menu_scene_draw  (Context* c, Image* buf);
