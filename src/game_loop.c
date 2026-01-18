#include "game_loop.h"

#include "config.h"
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
   if (!load_fixed_fonts (&c->fonts->fixed_font, &c->fonts->inverted_fixed_font,
                          FIXED_FONT_PATH))
      ERRNO_RETURN (false, "load fixed fonts");

   if (!set_current_scene (c, MAIN_MENU_BG_SCENE))
      ERROR_RETURN (false, "set current scene");

   if (!init_active_modals ())
      ERROR_RETURN (false, "init active modals");

   if (!init_active_overlays ())
      ERROR_RETURN (false, "init active overlays");

   add_modal (c, MAIN_MENU_MODAL);
   add_overlay (c, LOG_OVERLAY);
   add_overlay (c, OSD_OVERLAY);

   UILayer* overlay = get_overlay (OSD_OVERLAY);
   overlay->is_visible = true;

   return true;
}

bool
game_deinit (Context* c)
{
   if (!deinit_active_overlays ())
      ERROR_RETURN (false, "deinit active overlays");

   if (!deinit_active_modals ())
      ERROR_RETURN (false, "deinit active modals");

   if (!deinit_current_scene ())
      ERROR_RETURN (false, "deinit current scene");

   unload_fixed_font_images ();

   return true;
}

void
game_update (Context* c, float dt)
{
   update_input (c);
   update_state (c);

   update_current_scene (c, dt);
   update_active_modals (c, dt);
   update_active_overlays (c, dt);
}

void
game_draw (Context* c, Image* buf)
{
   draw_current_scene (c, buf);
   draw_active_modals (c, buf);
   draw_active_overlays (c, buf);
}
