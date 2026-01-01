#pragma once

#include "../context.h"

#include <raylib.h>

void main_menu_bg_scene_init (Context* c);
void main_menu_bg_scene_deinit ();
void main_menu_bg_scene_update (Context* c, float dt);
void main_menu_bg_scene_draw (Context* c, Image* buf);
