#pragma once

#include <stddef.h>
#include <raylib.h>

#include "ui_layer.h"
#include "context.h"

typedef struct active_ui_layers {
  UILayer** array;
  size_t    size;
  size_t    capacity;
} ActiveUILayers;

void init_active_ui_layers  (ActiveUILayers* active_ui_layers, size_t layer_count);
void init_active_modals     ();
void init_active_overlays   ();
void deinit_active_ui_layers(ActiveUILayers* active_ui_layers);
void deinit_active_modals   ();
void deinit_active_overlays ();
void add_active_ui_layer    (Context* c, ActiveUILayers* active_ui_layers, UILayer* ui_layer);
void add_modal              (Context* c, ModalID modal_id);
void add_overlay            (Context* c, OverlayID overlay_id);
void remove_active_ui_layer (ActiveUILayers* active_ui_layers, UILayer* ui_layer);
void remove_modal           (ModalID modal_id);
void remove_overlay         (OverlayID overlay_id);
void update_active_ui_layers(Context* c, ActiveUILayers* active_ui_layers, float dt);
void update_active_modals   (Context* c, float dt);
void update_active_overlays (Context* c, float dt);
void draw_active_ui_layers  (Context* c, ActiveUILayers* active_ui_layers, Image* buf);
void draw_active_modals     (Context* c, Image* buf);
void draw_active_overlays   (Context* c, Image* buf);

UILayer* get_overlay(OverlayID overlay_id);
