#include "ui_layer_manager.h"

#include "log.h"
#include "modals/main_menu_modal.h"
#include "overlays/log_overlay.h"
#include "overlays/osd_overlay.h"
#include "ui_layer.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

static UILayer s_modals[] = {
  [MAIN_MENU_MODAL] = {
    .init   = main_menu_modal_init,
    .deinit = main_menu_modal_deinit,
    .update = main_menu_modal_update,
    .draw   = main_menu_modal_draw,
  },
};
static UILayer s_overlays[] = {
  [LOG_OVERLAY] = {
    .init   = log_overlay_init,
    .deinit = log_overlay_deinit,
    .update = log_overlay_update,
    .draw   = log_overlay_draw,
  },
  [OSD_OVERLAY] = {
    .init   = osd_overlay_init,
    .deinit = osd_overlay_deinit,
    .update = osd_overlay_update,
    .draw   = osd_overlay_draw,
  }
};

static const char* s_modal_names[MODAL_COUNT] = {
   [MAIN_MENU_MODAL] = "main menu modal",
};

static const char* s_overlay_names[OVERLAY_COUNT] = {
   [LOG_OVERLAY] = "log overlay",
   [OSD_OVERLAY] = "osd overlay",
};

static ActiveUILayers s_active_modals;
static ActiveUILayers s_active_overlays;

bool
init_active_ui_layers (ActiveUILayers* active_ui_layers, size_t layer_count)
{
   UILayer** ui_layers = calloc (layer_count, sizeof (UILayer*));
   if (ui_layers == NULL)
      ERRNO_RETURN (false, "calloc");

   *active_ui_layers = (ActiveUILayers){
      .array    = ui_layers,
      .size     = 0,
      .capacity = layer_count,
   };

   return true;
}

bool
init_active_modals (void)
{
   assert (sizeof (s_modals) / sizeof (s_modals[0]) == MODAL_COUNT);

   return init_active_ui_layers (&s_active_modals, MODAL_COUNT);
}

bool
init_active_overlays (void)
{
   assert (sizeof (s_overlays) / sizeof (s_overlays[0]) == OVERLAY_COUNT);

   return init_active_ui_layers (&s_active_overlays, OVERLAY_COUNT);
}

bool
deinit_active_ui_layers (ActiveUILayers* active_ui_layers)
{
   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      UILayer* layer = active_ui_layers->array[i];
      if (!layer->deinit ())
         ERROR_RETURN (false, "deinit");
   }

   free (active_ui_layers->array);
   active_ui_layers->size = 0;

   return true;
}

bool
deinit_active_modals (void)
{
   return deinit_active_ui_layers (&s_active_modals);
}

bool
deinit_active_overlays (void)
{
   return deinit_active_ui_layers (&s_active_overlays);
}

bool
add_active_ui_layer (Context* c, ActiveUILayers* active_ui_layers,
                     UILayer* ui_layer)
{
   assert (active_ui_layers->capacity > 0);
   assert (active_ui_layers->size < active_ui_layers->capacity);

   // TODO: for larger arrays a map would be better
   for (size_t i = 0; i < active_ui_layers->size; ++i)
      if (active_ui_layers->array[i] == ui_layer)
         WARNING_RETURN (true, "UI layer '%s' is already added at position %d",
                         get_ui_layer_name (ui_layer), (int)i);

   active_ui_layers->array[active_ui_layers->size] = ui_layer;
   active_ui_layers->size++;

   return ui_layer->init (c);
}

bool
add_modal (Context* c, ModalID modal_id)
{
   assert (modal_id >= 0);
   assert (modal_id < MODAL_COUNT);

   return add_active_ui_layer (c, &s_active_modals, &s_modals[modal_id]);
}

bool
add_overlay (Context* c, OverlayID overlay_id)
{
   assert (overlay_id >= 0);
   assert (overlay_id < OVERLAY_COUNT);

   return add_active_ui_layer (c, &s_active_overlays, &s_overlays[overlay_id]);
}

bool
remove_active_ui_layer (ActiveUILayers* active_ui_layers, UILayer* ui_layer)
{

   if (active_ui_layers->size == 0)
      WARNING_RETURN (true,
                      "trying to remove UI layer '%s', "
                      "but there are no active UI layers to remove",
                      get_ui_layer_name (ui_layer));

   int found_at = -1;

   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      UILayer* layer = active_ui_layers->array[i];

      if (layer == ui_layer)
      {
         found_at = (int)i;

         if (!layer->deinit ())
            ERROR_RETURN (false, "deinit");

         break;
      }
   }

   if (found_at < 0)
      WARNING_RETURN (true, "UI layer '%s' not found in active layers",
                      get_ui_layer_name (ui_layer));

   for (size_t i = (size_t)found_at; i < active_ui_layers->size - 1; ++i)
      active_ui_layers[i] = active_ui_layers[i + 1];

   active_ui_layers->size--;

   return true;
}

bool
remove_modal (ModalID modal_id)
{
   assert (modal_id >= 0);
   assert (modal_id < MODAL_COUNT);

   return remove_active_ui_layer (&s_active_modals, &s_modals[modal_id]);
}

bool
remove_overlay (OverlayID overlay_id)
{
   assert (overlay_id >= 0);
   assert (overlay_id < OVERLAY_COUNT);

   return remove_active_ui_layer (&s_active_overlays, &s_overlays[overlay_id]);
}

void
update_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers, float dt)
{
   assert (active_ui_layers->capacity > 0);

   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      UILayer* layer = active_ui_layers->array[i];
      layer->update (c, dt);
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
draw_active_ui_layers (Context* c, ActiveUILayers* active_ui_layers)
{
   assert (active_ui_layers->capacity > 0);

   for (size_t i = 0; i < active_ui_layers->size; ++i)
   {
      UILayer* layer = active_ui_layers->array[i];
      layer->draw (c);
   }
}

void
draw_active_modals (Context* c)
{
   draw_active_ui_layers (c, &s_active_modals);
}

void
draw_active_overlays (Context* c)
{
   draw_active_ui_layers (c, &s_active_overlays);
}

const char*
get_ui_layer_name (UILayer* ui_layer)
{
   for (size_t i = 0; i < MODAL_COUNT; ++i)
      if (s_modals[i].init == ui_layer->init)
         return s_modal_names[i];

   for (size_t i = 0; i < OVERLAY_COUNT; ++i)
      if (s_overlays[i].init == ui_layer->init)
         return s_overlay_names[i];

   return "unknown UI layer";
}
