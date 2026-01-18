#pragma once

#include "context.h"

#include <raylib.h>

typedef struct ui_layer {
   const char* name;
   bool        is_visible;

   bool (*init) (struct ui_layer* self, Context* c);
   bool (*deinit) (struct ui_layer* self);
   void (*update) (struct ui_layer* self, Context* c, float dt);
   void (*draw) (struct ui_layer* self, Context* c, Image* buf);
} UILayer;

typedef enum overlay_id {
   LOG_OVERLAY = 0,
   OSD_OVERLAY,
   OVERLAY_COUNT,
} OverlayID;

typedef enum modal_id {
   MAIN_MENU_MODAL = 0,
   MODAL_COUNT,
} ModalID;
