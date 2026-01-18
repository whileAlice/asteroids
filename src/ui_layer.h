#pragma once

#include "context.h"

#include <raylib.h>

typedef struct ui_layer {
   bool (*init) (Context* c);
   bool (*deinit) (void);
   void (*update) (Context* c, float dt);
   void (*draw) (Context* c);
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
