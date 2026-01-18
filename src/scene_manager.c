#include "scene_manager.h"

#include "scene.h"
#include "scenes/demo_scene.h"
#include "scenes/empty_scene.h"
#include "scenes/vector_products_scene.h"

#include <assert.h>
#include <raylib.h>

static Scene s_scenes[] = {
  [EMPTY_SCENE] = {
    .init   = empty_scene_init,
    .update = empty_scene_update,
    .draw   = empty_scene_draw,
    .deinit = empty_scene_deinit,
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

bool
set_current_scene (Context* c, SceneID scene_id)
{
   assert (sizeof (s_scenes) / sizeof (s_scenes[0]) == SCENE_COUNT);
   assert (scene_id >= 0);
   assert (scene_id < SCENE_COUNT);

   if (s_current_scene != NULL)
      if (!s_current_scene->deinit (c))
         ERROR_RETURN (false, "deinit");

   s_current_scene = &s_scenes[scene_id];
   if (!s_current_scene->init (c))
      ERROR_RETURN (false, "init");

   return true;
}

bool
deinit_current_scene (Context* c)
{
   return s_current_scene->deinit (c);
}

void
update_current_scene (Context* c, float dt)
{
   assert (s_current_scene != NULL);

   s_current_scene->update (c, dt);
}

void
draw_current_scene (Context* c)
{
   assert (s_current_scene != NULL);

   s_current_scene->draw (c);
}

bool
is_current_scene (SceneID scene_id)
{
   assert (s_current_scene != NULL);

   return s_current_scene == &s_scenes[scene_id];
}
