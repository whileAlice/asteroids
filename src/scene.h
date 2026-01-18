#pragma once

#include "context.h"

typedef enum scene_id {
   MAIN_MENU_BG_SCENE = 0,
   DEMO_SCENE,
   VECTOR_PRODUCTS_SCENE,
   SCENE_COUNT,
} SceneID;

typedef struct scene {
   bool (*init) (Context* c);
   bool (*deinit) (void);
   void (*update) (Context* c, float dt);
   void (*draw) (Context* c, Image* buf);
} Scene;
