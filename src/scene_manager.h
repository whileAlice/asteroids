#pragma once

#include "context.h"
#include "scene.h"

typedef void (*init_func)(Context* c);
typedef void (*update_func)(Context* c, float dt);
typedef void (*draw_func)(Context* c, Image* buf);
typedef void (*deinit_func)();

typedef struct scene_functions{
  init_func   init;
  update_func update;
  draw_func   draw;
  deinit_func deinit;
} SceneFunctions;

void add_scene(Context* c, Scene scene);
void add_scene_before(Context* c, Scene new_scene, Scene existing_scene);
void remove_scene(Context* c, Scene scene);
void replace_scene(Context* c, Scene new_scene, Scene old_scene);
void init_active_scenes();
void deinit_active_scenes();
void update_active_scenes(Context* c, float dt);
void draw_active_scenes(Context* c, Image* buf);

init_func   get_scene_init_func(Scene scene);
update_func get_scene_update_func(Scene scene);
draw_func   get_scene_draw_func(Scene scene);
deinit_func get_scene_deinit_func(Scene scene);

void init_scene(Context* c, Scene scene);
void update_scene(Context* c, Scene scene, float dt);
void draw_scene(Context* c, Scene scene, Image* buf);
void deinit_scene(Scene scene);
