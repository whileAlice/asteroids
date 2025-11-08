#include <raylib.h>

#include "../context.h"
#include "../scene_manager.h"

static bool e = false;

void menu_init(Context* c){}
void menu_update(Context* c, float dt){
  if (IsKeyPressed(KEY_ESCAPE)) {
    if (!e) {
      replace_scene(c, DEMO_SCENE, VECTOR_PRODUCTS_SCENE);
      e = true;
    } else {
      replace_scene(c, VECTOR_PRODUCTS_SCENE, DEMO_SCENE);
      e = false;
    }
  }
}
void menu_draw(Context* c, Image* buf){}
void menu_deinit(){}
