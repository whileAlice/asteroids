#include "game_loop.h"

#include "config.h"
#include "context.h"
#include "scene_manager.h"
#include "ui_layer_manager.h"
#include "update_input.h"
#include "update_state.h"

#include <raylib.h>

void
game_init (Context* c)
{
   load_fixed_fonts (&c->fonts->fixed_font, &c->fonts->inverted_fixed_font,
                     FIXED_FONT_PATH);
   set_current_scene (c, MAIN_MENU_BG_SCENE);

   init_active_modals ();
   init_active_overlays ();

   add_modal (c, MAIN_MENU_MODAL);
   add_overlay (c, LOG_OVERLAY);
   add_overlay (c, OSD_OVERLAY);

   get_overlay (OSD_OVERLAY)->is_visible = true;
}

void
game_deinit (Context* c)
{
   deinit_active_overlays ();
   deinit_active_modals ();
   deinit_current_scene ();

   unload_fixed_font_images ();
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
