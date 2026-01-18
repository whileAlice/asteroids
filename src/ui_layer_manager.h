#pragma once

#include "context.h"
#include "ui_layer.h"

#include <raylib.h>
#include <stddef.h>

typedef struct active_ui_layers {
   UILayer** array;
   size_t    size;
   size_t    capacity;
} ActiveUILayers;

// clang-format off
bool        init_active_ui_layers   (ActiveUILayers* active_ui_layers, size_t layer_count);
bool        init_active_modals      (void);
bool        init_active_overlays    (void);
bool        deinit_active_ui_layers (ActiveUILayers* active_ui_layers);
bool        deinit_active_modals    (void);
bool        deinit_active_overlays  (void);
bool        add_active_ui_layer     (Context* c, ActiveUILayers* active_ui_layers, UILayer* ui_layer);
bool        add_modal               (Context* c, ModalID modal_id);
bool        add_overlay             (Context* c, OverlayID overlay_id);
bool        remove_active_ui_layer  (ActiveUILayers* active_ui_layers, UILayer* ui_layer);
bool        remove_modal            (ModalID modal_id);
bool        remove_overlay          (OverlayID overlay_id);
void        update_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers, float dt);
void        update_active_modals    (Context* c, float dt);
void        update_active_overlays  (Context* c, float dt);
void        draw_active_ui_layers   (Context* c, ActiveUILayers* active_ui_layers);
void        draw_active_modals      (Context* c);
void        draw_active_overlays    (Context* c);
const char* get_ui_layer_name       (UILayer* ui_layer);
