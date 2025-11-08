#include <raylib.h>

#include "context.h"
#include "game_loop.h"
#include "scene_manager.h"
#include "update_state.h"
#include "update_input.h"

void
game_init(Context* c)
{
  init_active_scenes();
  add_scene(c, VECTOR_PRODUCTS_SCENE);
  add_scene(c, LOG_DISPLAY_SCENE);
  add_scene(c, MENU_SCENE);
}

void
game_update(Context* c, float dt)
{
  update_input(c);
  update_state(c);

  update_active_scenes(c, dt);
}

void
game_draw(Context* c, Image* buf)
{
  draw_active_scenes(c, buf);
}

void
game_deinit(Context* c)
{
  deinit_active_scenes();
}
