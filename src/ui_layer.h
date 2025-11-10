#pragma once

#include <raylib.h>

#include "context.h"

typedef enum ui_layer_id {
  MAIN_MENU_LAYER = 0,
  OSD_LAYER,
  UI_LAYER_COUNT,
} UILayerID;

typedef struct ui_layer {
  bool      is_interactive;

  void (*init)  (Context* c);
  void (*deinit)();
  void (*update)(Context* c, float dt);
  void (*draw)  (Context* c, Image* buf);
} UILayer;
