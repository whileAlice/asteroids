#include "main_menu_modal.h"

#include "../context.h"
#include "../draw_utils.h"
#include "../scene.h"
#include "../scene_manager.h"

#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define TITLE_TEXT           "Asteroids (WIP)"
#define DEMO_TEXT            "Graphics demonstration"
#define VECTOR_PRODUCTS_TEXT "Vector products showcase"
#define EXIT_TEXT            "Exit"

typedef enum menu_option {
   DEMO_OPTION = 0,
   VECTOR_PRODUCTS_OPTION,
   EXIT_OPTION,
   OPTION_COUNT,
} MenuOption;

static bool       s_should_show_menu;
static MenuOption s_menu_option;
static Vector2    s_selector_offset = { .x = -10.f, .y = 0.f };

void
select_option (Context* c)
{
   switch (s_menu_option)
   {
   case DEMO_OPTION:
      if (!is_current_scene (DEMO_SCENE))
      {
         set_current_scene (c, DEMO_SCENE);
         s_should_show_menu = false;
      }

      break;
   case VECTOR_PRODUCTS_OPTION:
      if (!is_current_scene (VECTOR_PRODUCTS_SCENE))
      {
         set_current_scene (c, VECTOR_PRODUCTS_SCENE);
         s_should_show_menu = false;
      }

      break;
   case EXIT_OPTION: c->state->should_exit_app = true; break;
   default:
      // TODO: implement unreachable
      assert (1 == 0);
      exit (1);
   }
}

void
next_option ()
{
   if (s_menu_option < OPTION_COUNT - 1)
   {
      s_menu_option++;
   }
}

void
prev_option ()
{
   if (s_menu_option > 0)
   {
      s_menu_option--;
   }
}

void
main_menu_modal_init (UILayer* self, Context* c)
{
   s_should_show_menu = true;
   s_menu_option      = DEMO_OPTION;
}

void
main_menu_modal_deinit (UILayer* self)
{
}

void
main_menu_modal_update (UILayer* self, Context* c, float dt)
{
   if (s_should_show_menu)
   {
      if (IsKeyPressed (KEY_DOWN))
      {
         next_option ();
      }
      if (IsKeyPressed (KEY_UP))
      {
         prev_option ();
      }
      if (IsKeyPressed (KEY_ENTER))
      {
         select_option (c);
      }
   }

   if (IsKeyPressed (KEY_ESCAPE))
   {
      if (!is_current_scene (MAIN_MENU_BG_SCENE))
      {
         s_should_show_menu = !s_should_show_menu;
      }
   }
}

void
main_menu_modal_draw (UILayer* self, Context* c, Image* buf)
{
   if (s_should_show_menu)
   {
      draw_text_center (buf, &c->fonts->fixed_font, (Vector2){ .x = 0, .y = 40 },
                        TITLE_TEXT);

      Vector2 demo_option_origin = draw_text_center (
         buf, &c->fonts->fixed_font, (Vector2){ .x = 0, .y = 60 }, DEMO_TEXT);

      if (s_menu_option == DEMO_OPTION)
      {
         Vector2 demo_selector_origin =
            Vector2Add (demo_option_origin, s_selector_offset);
         draw_text (buf, &c->fonts->fixed_font, demo_selector_origin, "*");
      }

      Vector2 vector_products_option_origin =
         draw_text_center (buf, &c->fonts->fixed_font,
                           (Vector2){ .x = 0, .y = 70 }, VECTOR_PRODUCTS_TEXT);

      if (s_menu_option == VECTOR_PRODUCTS_OPTION)
      {
         Vector2 vector_products_selector_origin =
            Vector2Add (vector_products_option_origin, s_selector_offset);
         draw_text (buf, &c->fonts->fixed_font, vector_products_selector_origin,
                    "*");
      }

      Vector2 exit_option_origin = draw_text_center (
         buf, &c->fonts->fixed_font, (Vector2){ .x = 0, .y = 80 }, EXIT_TEXT);

      if (s_menu_option == EXIT_OPTION)
      {
         Vector2 exit_selector_origin =
            Vector2Add (exit_option_origin, s_selector_offset);
         draw_text (buf, &c->fonts->fixed_font, exit_selector_origin, "*");
      }
   }
}
