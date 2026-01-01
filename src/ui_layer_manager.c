#include "ui_layer_manager.h"

#include "modals/main_menu_modal.h"
#include "overlays/log_overlay.h"
#include "overlays/osd_overlay.h"
#include "ui_layer.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

static UILayer s_modals[] = {
  [MAIN_MENU_MODAL] = {
    .name = "main menu modal",

    .init   = main_menu_modal_init,
    .deinit = main_menu_modal_deinit,
    .update = main_menu_modal_update,
    .draw   = main_menu_modal_draw,
  },
};
static UILayer s_overlays[] = {
  [LOG_OVERLAY] = {
    .name = "log overlay",

    .init   = log_overlay_init,
    .deinit = log_overlay_deinit,
    .update = log_overlay_update,
    .draw   = log_overlay_draw,
  },
  [OSD_OVERLAY] = {
    .name = "osd overlay",

    .init   = osd_overlay_init,
    .deinit = osd_overlay_deinit,
    .update = osd_overlay_update,
    .draw   = osd_overlay_draw,
  }
};

static ActiveUILayers s_active_modals;
static ActiveUILayers s_active_overlays;

void
init_active_ui_layers (ActiveUILayers* active_ui_layers, size_t layer_count)
{
   *active_ui_layers = (ActiveUILayers){
      .array    = malloc (layer_count * sizeof (UILayer*)),
      .size     = 0,
      .capacity = layer_count,
   };
}

void
init_active_modals ()
{
   assert (sizeof (s_modals) / sizeof (s_modals[0]) == MODAL_COUNT);

   init_active_ui_layers (&s_active_modals, MODAL_COUNT);
}

void
init_active_overlays ()
{
   assert (sizeof (s_overlays) / sizeof (s_overlays[0]) == OVERLAY_COUNT);

   init_active_ui_layers (&s_active_overlays, OVERLAY_COUNT);
}

void
deinit_active_ui_layers (ActiveUILayers* active_ui_layers)
{
   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      auto* layer = active_ui_layers->array[i];
      layer->deinit (layer);
   }

   free (active_ui_layers->array);
   active_ui_layers->size = 0;
}

void
deinit_active_modals ()
{
   deinit_active_ui_layers (&s_active_modals);
}

void
deinit_active_overlays ()
{
   deinit_active_ui_layers (&s_active_overlays);
}

void
add_active_ui_layer (Context* c, ActiveUILayers* active_ui_layers,
                     UILayer* ui_layer)
{
   assert (active_ui_layers->capacity > 0);
   assert (active_ui_layers->size < active_ui_layers->capacity);

   // TODO: for larger arrays a map would be better
   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      if (ui_layer == active_ui_layers->array[i])
      {
         // TODO: implement enum -> string conversion
         printf ("WARNING: UI layer '%s' is already added at position %d\n",
                 ui_layer->name, (int)i);

         return;
      }
   }

   active_ui_layers->array[active_ui_layers->size] = ui_layer;
   active_ui_layers->size++;

   ui_layer->init (ui_layer, c);
}

void
add_modal (Context* c, ModalID modal_id)
{
   assert (modal_id >= 0);
   assert (modal_id < MODAL_COUNT);

   add_active_ui_layer (c, &s_active_modals, &s_modals[modal_id]);
}

void
add_overlay (Context* c, OverlayID overlay_id)
{
   assert (overlay_id >= 0);
   assert (overlay_id < OVERLAY_COUNT);

   add_active_ui_layer (c, &s_active_overlays, &s_overlays[overlay_id]);
}

void
remove_active_ui_layer (ActiveUILayers* active_ui_layers, UILayer* ui_layer)
{

   if (active_ui_layers->size == 0)
   {
      printf ("WARNING: trying to remove UI layer '%s', "
              "but there are no active UI layers to remove\n",
              ui_layer->name);

      return;
   }

   int found_at = -1;

   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      auto* layer = active_ui_layers->array[i];

      if (layer == ui_layer)
      {
         found_at = (int)i;
         layer->deinit (layer);

         break;
      }
   }

   if (found_at < 0)
   {
      printf ("WARNING: UI layer '%s' not found in active layers\n",
              ui_layer->name);

      return;
   }

   for (size_t i = (size_t)found_at; i < active_ui_layers->size - 1; ++i)
   {
      active_ui_layers[i] = active_ui_layers[i + 1];
   }

   active_ui_layers->size--;
}

void
remove_modal (ModalID modal_id)
{
   assert (modal_id >= 0);
   assert (modal_id < MODAL_COUNT);

   remove_active_ui_layer (&s_active_modals, (UILayer*)&s_modals[modal_id]);
}

void
remove_overlay (OverlayID overlay_id)
{
   assert (overlay_id >= 0);
   assert (overlay_id < OVERLAY_COUNT);

   remove_active_ui_layer (&s_active_overlays,
                           (UILayer*)&s_overlays[overlay_id]);
}

void
update_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers, float dt)
{
   assert (active_ui_layers->capacity > 0);

   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      auto* layer = active_ui_layers->array[i];
      layer->update (layer, c, dt);
   }
}

void
update_active_modals (Context* c, float dt)
{
   update_active_ui_layers (c, &s_active_modals, dt);
}

void
update_active_overlays (Context* c, float dt)
{
   update_active_ui_layers (c, &s_active_overlays, dt);
}

void
draw_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers, Image* buf)
{
   assert (active_ui_layers->capacity > 0);

   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      auto* layer = active_ui_layers->array[i];
      layer->draw (layer, c, buf);
   }
}

void
draw_active_modals (Context* c, Image* buf)
{
   draw_active_ui_layers (c, &s_active_modals, buf);
}

void
draw_active_overlays (Context* c, Image* buf)
{
   draw_active_ui_layers (c, &s_active_overlays, buf);
}

UILayer*
get_overlay (OverlayID overlay_id)
{
   assert (overlay_id >= 0);
   assert (overlay_id < OVERLAY_COUNT);

   return &s_overlays[overlay_id];
}
