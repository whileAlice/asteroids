#include <assert.h>
#include <stdlib.h>

#include "scene_manager.h"
#include "scenes/demo.h"
#include "scenes/log_display.h"
#include "scenes/vector_products.h"
#include "scenes/menu.h"

static Scenes         s_active_scenes;
static SceneFunctions s_scene_functions[] = {
  [DEMO_SCENE] = {
    .init   = demo_init,
    .update = demo_update,
    .draw   = demo_draw,
    .deinit = demo_deinit,
  },
  [LOG_DISPLAY_SCENE] = {
    .init   = log_display_init,
    .update = log_display_update,
    .draw   = log_display_draw,
    .deinit = log_display_deinit,
  },
  [VECTOR_PRODUCTS_SCENE] = {
    .init   = vector_products_init,
    .update = vector_products_update,
    .draw   = vector_products_draw,
    .deinit = vector_products_deinit,
  },
  [MENU_SCENE] = {
    .init   = menu_init,
    .update = menu_update,
    .draw   = menu_draw,
    .deinit = menu_deinit,
  },
};

void
add_scene(Context* c, Scene scene)
{
  Scenes* s = &s_active_scenes;
  assert(s->array != NULL);
  assert(s->size < s->capacity);

  for (size_t i = 0; i < s->size; ++i) {
    if (s->array[i] == scene) {
      printf("WARNING: scene %d already added!\n", scene);
      return;
    }
  }

  s->array[s->size++] = scene;
  init_scene(c, scene);
}

void
add_scene_before(Context* c, Scene new_scene, Scene existing_scene)
{
  Scenes* s = &s_active_scenes;
  assert(s->array != NULL);
  assert(s->size < s->capacity);

  bool found = false;

  for (size_t i = 0; i < s->size; ++i) {
    if (s->array[i] == existing_scene) {
      found = true;
      s->size += 1;

      for (size_t j = s->size - 1; j > i; --j) {
        s->array[j] = s->array[j - 1];
      }

      s->array[i] = new_scene;
      init_scene(c, new_scene);

      break;
    }
  }

  if (!found) {
    printf("WARNING: scene %d not found in active scenes!\n", existing_scene);
  }
}

void
remove_scene(Context* c, Scene scene)
{
  Scenes* s = &s_active_scenes;
  assert(s->array != NULL);

  bool found = false;

  for (size_t i = 0; i < s->size; ++i) {
    if (found) {
      s->array[i - 1] = s->array[i];
    }
    if (s->array[i] == scene) {
      found = true;
      deinit_scene(scene);
    }
  }

  if (found) {
    s->size -= 1;
  } else {
    printf("WARNING: scene %d not found in active scenes!\n", scene);
  }
}

void
replace_scene(Context* c, Scene new_scene, Scene old_scene)
{
  Scenes* s = &s_active_scenes;
  assert(s->array != NULL);

  bool found = false;

  for (size_t i = 0; i < s->size; ++i) {
    if (s->array[i] == old_scene) {
      found = true;

      deinit_scene(old_scene);
      init_scene(c, new_scene);
      s->array[i] = new_scene;

      break;
    }
  }

  if (!found) {
    printf("WARNING: scene %d not found in active scenes!\n", old_scene);
  }
}

void
init_active_scenes()
{
  const size_t capacity = SCENE_COUNT * sizeof(Scene);

  s_active_scenes = (Scenes){
    .array    = malloc(capacity),
    .size     = 0,
    .capacity = capacity,
  };
}

void
deinit_active_scenes() {
  for (int i = (int)s_active_scenes.size - 1; i >= 0; --i) {
    deinit_scene(s_active_scenes.array[i]);
  }

  free(s_active_scenes.array);
}

void
update_active_scenes(Context* c, float dt)
{
  for (size_t i = 0; i < s_active_scenes.size; ++i) {
    update_scene(c, s_active_scenes.array[i], dt);
  }
}

void
draw_active_scenes(Context* c, Image* buf)
{
  for (size_t i = 0; i < s_active_scenes.size; ++i) {
    draw_scene(c, s_active_scenes.array[i], buf);
  }
}

init_func
get_scene_init_func(Scene scene)
{
  if (scene >= 0 && scene < SCENE_COUNT) {
    return s_scene_functions[scene].init;
  } else {
    printf("ERROR: scene %d does not exist\n", scene);
    exit(1);
  }
}

update_func
get_scene_update_func(Scene scene)
{
  if (scene >= 0 && scene < SCENE_COUNT) {
    return s_scene_functions[scene].update;
  } else {
    printf("ERROR: scene %d does not exist\n", scene);
    exit(1);
  }
}

draw_func
get_scene_draw_func(Scene scene)
{
  if (scene >= 0 && scene < SCENE_COUNT) {
    return s_scene_functions[scene].draw;
  } else {
    printf("ERROR: scene %d does not exist\n", scene);
    exit(1);
  }
}

deinit_func
get_scene_deinit_func(Scene scene)
{
  if (scene >= 0 && scene < SCENE_COUNT) {
    return s_scene_functions[scene].deinit;
  } else {
    printf("ERROR: scene %d does not exist\n", scene);
    exit(1);
  }
}

void
init_scene(Context* c, Scene scene)
{
  auto init_func = get_scene_init_func(scene);
  if (init_func != NULL) {
    init_func(c);
  } else {
    printf("ERROR: scene %d init not found\n", scene);
    exit(1);
  }
}

void
update_scene(Context* c, Scene scene, float dt)
{
  auto update_func = get_scene_update_func(scene);
  if (update_func != NULL) {
    update_func(c, dt);
  } else {
    printf("ERROR: scene %d update not found\n", scene);
    exit(1);
  }
}

void
draw_scene(Context* c, Scene scene, Image* buf)
{
  auto draw_func = get_scene_draw_func(scene);
  if (draw_func != NULL) {
    draw_func(c, buf);
  } else {
    printf("ERROR: scene %d draw not found\n", scene);
    exit(1);
  }
}

void
deinit_scene(Scene scene)
{
  auto deinit_func = get_scene_deinit_func(scene);
  if (deinit_func != NULL) {
    deinit_func();
  } else {
    printf("ERROR: scene %d deinit not found\n", scene);
    exit(1);
  }
}
