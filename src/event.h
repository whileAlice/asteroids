#pragma once

#include "context.h"
#include "scene.h"

// clang-format off
bool event_handle      (Context* c);
void app_quit_initiate (Context* c);
bool should_quit_app   (Context* c);
void change_scene      (Context* c, SceneID new_scene);
