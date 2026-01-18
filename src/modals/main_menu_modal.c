#include "main_menu_modal.h"

#include "../context.h"
#include "../draw_utils.h"
#include "../event.h"
#include "../scene.h"
#include "../scene_manager.h"

#include <assert.h>
#include <raylib.h>
#include <raymath.h>

#define TITLE_TEXT           "Asteroids (WIP)"
#define DEMO_TEXT            "Graphics demonstration"
#define VECTOR_PRODUCTS_TEXT "Vector products showcase"
#define EXIT_TEXT            "Exit"

#define SELECTOR_OFFSET 10

typedef enum menu_option {
   DEMO_OPTION = 0,
   VECTOR_PRODUCTS_OPTION,
   EXIT_OPTION,
   OPTION_COUNT,
} MenuOption;

static bool       s_should_show_menu;
static MenuOption s_menu_option;
static SceneID    s_new_scene;

bool
main_menu_modal_init (Context* c)
{
   s_should_show_menu = true;
   s_menu_option      = DEMO_OPTION;

   return true;
}

bool
main_menu_modal_deinit (void)
{
   return true;
}

void
main_menu_modal_update (Context* c, float dt)
{
   if (s_should_show_menu)
   {
      if (IsKeyPressed (KEY_DOWN))
         next_option ();

      if (IsKeyPressed (KEY_UP))
         prev_option ();

      if (IsKeyPressed (KEY_ENTER))
      {
         select_option (c);
         if (!is_current_scene (s_new_scene))
            change_scene (c, s_new_scene);
      }
   }

   if (IsKeyPressed (KEY_ESCAPE))
   {
      if (!is_current_scene (MAIN_MENU_BG_SCENE))
         s_should_show_menu = !s_should_show_menu;
   }
}

void
main_menu_modal_draw (Context* c)
{
   if (!s_should_show_menu)
      return;

   const int buffer_width = c->pixel_buffer->image->width;

   draw_text_center_i (0, 40, buffer_width, TITLE_TEXT);

   int demo_option_origin_x =
      draw_text_center_i (0, 60, buffer_width, DEMO_TEXT);

   if (s_menu_option == DEMO_OPTION)
      draw_text_i (demo_option_origin_x - SELECTOR_OFFSET, 60, "*");

   int vector_products_option_origin_x =
      draw_text_center_i (0, 70, buffer_width, VECTOR_PRODUCTS_TEXT);

   if (s_menu_option == VECTOR_PRODUCTS_OPTION)
      draw_text_i (vector_products_option_origin_x - SELECTOR_OFFSET, 70, "*");

   int exit_option_origin_x =
      draw_text_center_i (0, 80, buffer_width, EXIT_TEXT);

   if (s_menu_option == EXIT_OPTION)
      draw_text_i (exit_option_origin_x - SELECTOR_OFFSET, 80, "*");
}

void
select_option (Context* c)
{
   switch (s_menu_option)
   {
      case DEMO_OPTION           : s_new_scene = DEMO_SCENE; break;
      case VECTOR_PRODUCTS_OPTION: s_new_scene = VECTOR_PRODUCTS_SCENE; break;
      case EXIT_OPTION           : app_quit_initiate (c); break;
      default:
         // TODO: implement unreachable
         assert (0 == "Unreachable");
   }

   s_should_show_menu = false;
}

void
next_option ()
{
   if (s_menu_option < OPTION_COUNT - 1)
      s_menu_option++;
}

void
prev_option ()
{
   if (s_menu_option > 0)
      s_menu_option--;
}
