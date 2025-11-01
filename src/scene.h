#pragma once

#include <stddef.h>

typedef enum scene{
  DEMO_SCENE = 0,
  LOG_DISPLAY_SCENE,
  SCENE_COUNT,
} Scene;

typedef struct scenes{
  Scene* array;
  size_t size;
  size_t capacity;
} Scenes;
