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

void init_active_ui_layers  ();
void deinit_active_ui_layers();
void add_ui_layer           (Context* c, UILayerID layer);
void remove_ui_layer        (UILayerID layer);
void update_active_ui_layers(Context* c, float dt);
void draw_active_ui_layers  (Context* c, Image* buf);
