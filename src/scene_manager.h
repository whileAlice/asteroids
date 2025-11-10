#pragma once

#include "scene.h"
#include "context.h"

void set_current_scene   (Context* c, SceneID scene_id);
void deinit_current_scene();
void update_current_scene(Context* c, float dt);
void draw_current_scene  (Context* c, Image* buf);
bool is_current_scene(SceneID scene_id);
