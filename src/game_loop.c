#include "game_loop.h"

#include "context.h"
#include "error.h"
#include "scene_manager.h"
#include "ui_layer_manager.h"
#include "update_input.h"
#include "update_state.h"

#include <raylib.h>

bool
game_init (Context* c)
{
   if (!set_current_scene (c, EMPTY_SCENE))
      ERROR_RETURN (false, "set empty scene");

   if (!add_modal (c, MAIN_MENU_MODAL))
      ERROR_RETURN (false, "add main menu modal");

   if (!add_overlay (c, LOG_OVERLAY))
      ERROR_RETURN (false, "add log overlay");

   if (!add_overlay (c, OSD_OVERLAY))
      ERROR_RETURN (false, "add osd overlay");

   c->state->should_show_osd = true;

   return true;
}

bool
game_deinit (Context* c)
{
   if (!deinit_active_overlays (c))
      ERROR_RETURN (false, "deinit active overlays");

   if (!deinit_active_modals (c))
      ERROR_RETURN (false, "deinit active modals");

   if (!deinit_current_scene (c))
      ERROR_RETURN (false, "deinit current scene");

   return true;
}

void
game_update (Context* c, float dt)
{
   update_input (c);
   update_state (c);

   if (!c->state->is_paused)
      update_current_scene (c, dt);

   update_active_modals (c, dt);
   update_active_overlays (c, dt);
}

void
game_draw (Context* c)
{
   draw_current_scene (c);
   draw_active_modals (c);
   draw_active_overlays (c);
}
