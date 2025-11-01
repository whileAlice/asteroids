#include <assert.h>
#include <stdlib.h>

#include "scene_manager.h"
#include "scenes/demo.h"
#include "scenes/log_display.h"

static Scenes s_active_scenes;

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
  for (size_t i = 0; i < s_active_scenes.size; ++i) {
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
  switch(scene) {
  case DEMO_SCENE:
    return &demo_init;
  case LOG_DISPLAY_SCENE:
    return &log_display_init;
  default:
    return NULL;
  }
}

update_func
get_scene_update_func(Scene scene)
{
  switch(scene) {
  case DEMO_SCENE:
    return &demo_update;
  case LOG_DISPLAY_SCENE:
    return &log_display_update;
  default:
    return NULL;
  }
}

draw_func
get_scene_draw_func(Scene scene)
{
  switch(scene) {
  case DEMO_SCENE:
    return &demo_draw;
  case LOG_DISPLAY_SCENE:
    return &log_display_draw;
  default:
    return NULL;
  }
}

deinit_func
get_scene_deinit_func(Scene scene)
{
  switch(scene) {
  case DEMO_SCENE:
    return &demo_deinit;
  case LOG_DISPLAY_SCENE:
    return &log_display_deinit;
  default:
    return NULL;
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
