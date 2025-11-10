#include <assert.h>
#include <raylib.h>

#include "scene_manager.h"
#include "scene.h"
#include "scenes/main_menu.h"
#include "scenes/demo.h"
#include "scenes/vector_products.h"

static Scene s_scenes[] = {
  [MAIN_MENU_SCENE] = {
    .init   = main_menu_scene_init,
    .update = main_menu_scene_update,
    .draw   = main_menu_scene_draw,
    .deinit = main_menu_scene_deinit,
  },
  [DEMO_SCENE] = {
    .init   = demo_scene_init,
    .update = demo_scene_update,
    .draw   = demo_scene_draw,
    .deinit = demo_scene_deinit,
  },
  [VECTOR_PRODUCTS_SCENE] = {
    .init   = vector_products_scene_init,
    .update = vector_products_scene_update,
    .draw   = vector_products_scene_draw,
    .deinit = vector_products_scene_deinit,
  },
};
static Scene* s_current_scene;

void
set_current_scene(Context* c, SceneID scene_id)
{
  assert(sizeof(s_scenes) / sizeof(s_scenes[0]) == SCENE_COUNT);
  assert(scene_id >= 0);
  assert(scene_id < SCENE_COUNT);

  if (s_current_scene != NULL) {
    s_current_scene->deinit();
  }

  s_current_scene = &s_scenes[scene_id];
  s_current_scene->init(c);
}

void
deinit_current_scene()
{
  s_current_scene->deinit();
}

void
update_current_scene(Context* c, float dt)
{
  assert(s_current_scene != NULL);

  s_current_scene->update(c, dt);
}

void
draw_current_scene(Context* c, Image* buf)
{
  assert(s_current_scene != NULL);

  s_current_scene->draw(c, buf);
}

bool
is_current_scene(SceneID scene_id)
{
  assert(s_current_scene != NULL);

  return s_current_scene == &s_scenes[scene_id];
}
