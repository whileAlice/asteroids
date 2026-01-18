#pragma once

#include "context.h"
#include "scene.h"

// clang-format off
bool set_current_scene    (Context* c, SceneID scene_id);
bool deinit_current_scene (void);
void update_current_scene (Context* c, float dt);
void draw_current_scene   (Context* c, Image* buf);
bool is_current_scene     (SceneID scene_id);
