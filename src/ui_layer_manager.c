#include <stdlib.h>
#include <assert.h>
#include <raylib.h>

#include "ui_layer_manager.h"
#include "ui_layer.h"
#include "ui_layers/osd.h"
#include "ui_layers/main_menu.h"

static UILayer s_ui_layers[] = {
  [MAIN_MENU_LAYER] = {
    .is_interactive = true,

    .init   = main_menu_layer_init,
    .deinit = main_menu_layer_deinit,
    .update = main_menu_layer_update,
    .draw   = main_menu_layer_draw,
  },
  [OSD_LAYER] = {
    .is_interactive = false,

    .init   = osd_layer_init,
    .deinit = osd_layer_deinit,
    .update = osd_layer_update,
    .draw   = osd_layer_draw,
  }
};

static ActiveUILayers s_active_ui_layers;

void
init_active_ui_layers()
{
  assert(sizeof(s_ui_layers) / sizeof(s_ui_layers[0]) == UI_LAYER_COUNT);

  s_active_ui_layers = (ActiveUILayers){
    .array    = malloc(UI_LAYER_COUNT * sizeof(UILayer*)),
    .size     = 0,
    .capacity = UI_LAYER_COUNT,
  };
}

void
deinit_active_ui_layers()
{
  for (size_t i = 0; i < s_active_ui_layers.size; ++i) {
    s_active_ui_layers.array[i]->deinit();
  }

  free(s_active_ui_layers.array);
  s_active_ui_layers.size = 0;
}

void
add_ui_layer(Context* c, UILayerID layer_id)
{
  auto* layer = &s_ui_layers[layer_id];
  auto* active_layers = &s_active_ui_layers;

  assert(active_layers->capacity > 0);
  assert(active_layers->size < active_layers->capacity);
  assert(layer_id >= 0);
  assert(layer_id < UI_LAYER_COUNT);

  // TODO: for larger arrays a map would be better
  for (size_t i = 0; i < active_layers->size; ++i) {
    if (layer == active_layers->array[i]) {
      // TODO: implement enum -> string conversion
      printf("WARNING: UI layer %d is already added at position %d\n",
             layer_id, (int)i);

      return;
    }
  }

  active_layers->array[active_layers->size] = layer;
  active_layers->size++;

  layer->init(c);
}

void
remove_ui_layer(UILayerID layer_id)
{
  assert(layer_id >= 0);
  assert(layer_id < UI_LAYER_COUNT);

  auto* active_layers = &s_active_ui_layers;

  if (active_layers->size == 0) {
    printf("WARNING: no active UI layers to remove\n");

    return;
  }

  int found_at = -1;

  for (size_t i = 0; i < active_layers->size; ++i) {
    auto* layer = active_layers->array[i];

    if (layer == &s_ui_layers[layer_id]) {
      found_at = (int)i;
      layer->deinit();

      break;
    }
  }

  if (found_at < 0) {
    printf("WARNING: UI layer %d not found in active layers\n", layer_id);

    return;
  }

  for (size_t i = (size_t)found_at; i < active_layers->size - 1; ++i) {
    active_layers[i] = active_layers[i + 1];
  }

  active_layers->size--;
}

void
update_active_ui_layers(Context* c, float dt)
{
  assert(s_active_ui_layers.capacity > 0);

  for (size_t i = 0; i < s_active_ui_layers.size; ++i) {
    s_active_ui_layers.array[i]->update(c, dt);
  }
}

void
draw_active_ui_layers(Context* c, Image* buf)
{
  assert(s_active_ui_layers.capacity > 0);

  for (size_t i = 0; i < s_active_ui_layers.size; ++i) {
    s_active_ui_layers.array[i]->draw(c, buf);
  }
}
