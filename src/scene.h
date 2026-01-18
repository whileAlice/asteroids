#pragma once

typedef enum scene_id {
   EMPTY_SCENE = 0,
   DEMO_SCENE,
   VECTOR_PRODUCTS_SCENE,
   SCENE_COUNT,
} SceneID;

typedef struct context Context;

typedef struct scene {
   bool (*init) (Context* c);
   bool (*deinit) (Context* c);
   void (*update) (Context* c, float dt);
   void (*draw) (Context* c);
} Scene;
