#include <raylib.h>

#include "context.h"
#include "config.h"
#include "game_loop.h"
#include "scene_manager.h"
#include "ui_layer_manager.h"
#include "update_state.h"
#include "update_input.h"

void
game_init(Context* c)
{
  load_fixed_fonts(&c->font.fixed_font,
                   &c->font.inverted_fixed_font,
                   FIXED_FONT_PATH);

  set_current_scene(c, MAIN_MENU_SCENE);
  init_active_ui_layers();
  add_ui_layer(c, MAIN_MENU_LAYER);
  add_ui_layer(c, OSD_LAYER);
}

void
game_deinit(Context* c)
{
  deinit_active_ui_layers();
  deinit_current_scene();

  unload_fixed_font_images();
}

void
game_update(Context* c, float dt)
{
  update_input(c);
  update_state(c);
  update_current_scene(c, dt);
  update_active_ui_layers(c, dt);
}

void
game_draw(Context* c, Image* buf)
{
  draw_current_scene(c, buf);
  draw_active_ui_layers(c, buf);
}
