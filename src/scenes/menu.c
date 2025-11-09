#include <stdlib.h>
#include <assert.h>
#include <raylib.h>
#include <raymath.h>

#include "../context.h"
#include "../draw_utils.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "../log.h"

#define TITLE_TEXT           "Asteroids (WIP)"
#define DEMO_TEXT            "Graphics demonstration"
#define VECTOR_PRODUCTS_TEXT "Vector products showcase"
#define EXIT_TEXT            "Exit"

typedef enum menu_option{
  DEMO_OPTION = 0,
  VECTOR_PRODUCTS_OPTION,
  EXIT_OPTION,
  OPTION_COUNT,
}MenuOption;

static bool       s_should_show_menu;
static bool       s_is_running;
static MenuOption s_menu_option;
static Scene      s_current_scene;

static Vector2 s_selector_offset = { .x = -10.f, .y = 0.f };

void
select_option(Context* c)
{
  switch (s_menu_option) {
  case DEMO_OPTION:
    if (s_is_running) {
      assert(s_current_scene != MENU_SCENE);
      replace_scene(c, DEMO_SCENE, s_current_scene);
    } else {
      add_scene_before(c, DEMO_SCENE, MENU_SCENE);
    }

    s_current_scene = DEMO_SCENE;
    s_should_show_menu = false;
    s_is_running = true;

    break;
  case VECTOR_PRODUCTS_OPTION:
    if (s_is_running) {
      assert(s_current_scene != MENU_SCENE);
      replace_scene(c, VECTOR_PRODUCTS_SCENE, s_current_scene);
    } else {
      add_scene_before(c, VECTOR_PRODUCTS_SCENE, MENU_SCENE);
    }

    s_current_scene = VECTOR_PRODUCTS_SCENE;
    s_should_show_menu = false;
    s_is_running = true;

    break;
  case EXIT_OPTION:
    c->state.should_exit_app = true;

    break;
  default:
    // TODO: implement unreachable
    assert(1 == 0);
    exit(1);
  }

  // TODO: this needs a betters solution
  clear_osd_buffer(c);
}

void
next_option()
{
  if (s_menu_option < OPTION_COUNT - 1) {
    s_menu_option++;
  }
}

void
prev_option()
{
  if (s_menu_option > 0) {
    s_menu_option--;
  }
}

void
menu_init(Context* c)
{
  s_should_show_menu = true;
  s_current_scene = MENU_SCENE;
  s_menu_option = DEMO_OPTION;
}

void
menu_update(Context* c, float dt)
{
  if (IsKeyPressed(KEY_DOWN)) {
    next_option();
  }
  if (IsKeyPressed(KEY_UP)) {
    prev_option();
  }
  if (IsKeyPressed(KEY_ENTER)) {
    select_option(c);
  }
  if (IsKeyPressed(KEY_ESCAPE)) {
    if (s_is_running) {
      s_should_show_menu = !s_should_show_menu;
    }
  }
}

void
menu_draw(Context* c, Image* buf)
{
  if (s_should_show_menu) {
    clear_rgb_image(buf, rgb_from_rgba(PINK));

    draw_text_center(buf, &c->font.fixed_font,
                     (Vector2){ .x = 0, .y = 40 },
                     TITLE_TEXT);

    Vector2 demo_option_origin =
      draw_text_center(buf, &c->font.fixed_font,
                       (Vector2){ .x = 0, .y = 60 },
                       DEMO_TEXT);

    if (s_menu_option == DEMO_OPTION) {
      Vector2 demo_selector_origin =
        Vector2Add(demo_option_origin, s_selector_offset);
      draw_text(buf, &c->font.fixed_font, demo_selector_origin, "*");
    }

    Vector2 vector_products_option_origin =
      draw_text_center(buf, &c->font.fixed_font,
                       (Vector2){ .x = 0, .y = 70 },
                       VECTOR_PRODUCTS_TEXT);

    if (s_menu_option == VECTOR_PRODUCTS_OPTION) {
      Vector2 vector_products_selector_origin =
        Vector2Add(vector_products_option_origin, s_selector_offset);
      draw_text(buf, &c->font.fixed_font, vector_products_selector_origin, "*");
    }

    Vector2 exit_option_origin =
      draw_text_center(buf, &c->font.fixed_font,
                       (Vector2){ .x = 0, .y = 80 },
                       EXIT_TEXT);

    if (s_menu_option == EXIT_OPTION) {
      Vector2 exit_selector_origin =
        Vector2Add(exit_option_origin, s_selector_offset);
      draw_text(buf, &c->font.fixed_font, exit_selector_origin, "*");
    }
  }
}

void menu_deinit(){}
