#pragma once

#include "context.h"

typedef enum scene_id {
  MAIN_MENU_SCENE = 0,
  DEMO_SCENE,
  VECTOR_PRODUCTS_SCENE,
  SCENE_COUNT,
} SceneID;

typedef struct scene {
  void (*init)  (Context* c);
  void (*deinit)();
  void (*update)(Context* c, float dt);
  void (*draw)  (Context* c, Image* buf);
} Scene;
