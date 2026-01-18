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
bool active_ui_layers_init (ActiveUILayers* active_ui_layers, size_t layer_count);
bool active_modals_init (void);
bool active_overlays_init (void);
bool deinit_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers);
bool deinit_active_modals (Context* c);
bool deinit_active_overlays (Context* c);
bool add_active_ui_layer (Context* c, ActiveUILayers* active_ui_layers, UILayer* ui_layer);
bool add_modal (Context* c, ModalID modal_id);
bool add_overlay (Context* c, OverlayID overlay_id);
bool remove_active_ui_layer (Context* c, ActiveUILayers* active_ui_layers, UILayer* ui_layer);
bool remove_modal (Context* c, ModalID modal_id);
bool remove_overlay (Context* c, OverlayID overlay_id);
void update_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers, float dt);
void update_active_modals (Context* c, float dt);
void update_active_overlays (Context* c, float dt);
void draw_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers);
void draw_active_modals (Context* c);
void draw_active_overlays (Context* c);
const char* get_ui_layer_name (UILayer* ui_layer);
ModalID get_modal_id (UILayer* ui_layer);
bool is_modal_open (ModalID modal_id);
