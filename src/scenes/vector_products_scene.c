#include "vector_products_scene.h"

#include "../config.h"
#include "../context.h"
#include "../draw_utils.h"
#include "../math_utils.h"
#include "../window_utils.h"

#include <raylib.h>
#include <raymath.h>

// TODO: handle Y axis inversion in a clearer fashion
#define INVERT_Y(y) ((float)(PIXEL_BUFFER_HEIGHT - 1) - y)

typedef enum selected_point { A, B, C } SelectedPoint;

static Vector2       s_point_a, s_point_b, s_point_c;
static Color         s_color_a, s_color_b, s_color_c;
static SelectedPoint s_current_point, s_previous_point;
static float         s_cross_product_magnitude, s_dot_product;
static float         s_abc_angle_signed, s_abc_angle_360;
static bool          s_should_show_angle;

bool
vector_products_scene_init (Context* c)
{
   s_point_a = (Vector2){ .x = 23.f, .y = 40.f };
   s_point_b = (Vector2){ .x = 61.f, .y = 171.f };
   s_point_c = (Vector2){ .x = 112.f, .y = 65.f };

   s_color_a = RED;
   s_color_b = BLACK;
   s_color_c = BLACK;

   s_current_point  = A;
   s_previous_point = A;

   set_mouse_pos (c, s_point_a);

   return true;
}

bool
vector_products_scene_deinit (Context* c)
{
   return true;
}

void
vector_products_scene_update (Context* c, float dt)
{
   if (IsKeyPressed (KEY_SPACE))
      switch (s_current_point)
      {
         case A: s_current_point = B; break;
         case B: s_current_point = C; break;
         case C: s_current_point = A; break;
      }

   if (IsKeyPressed (KEY_ONE) || IsKeyPressed (KEY_A))
      s_current_point = A;

   if (IsKeyPressed (KEY_TWO) || IsKeyPressed (KEY_B))
      s_current_point = B;

   if (IsKeyPressed (KEY_THREE) || IsKeyPressed (KEY_C))
      s_current_point = C;

   int     mouse_x   = get_mouse_x (c);
   int     mouse_y   = get_mouse_y (c);
   Vector2 mouse_pos = {
      .x = (float)mouse_x,
      .y = (float)mouse_y,
   };

   // TODO: this is a bit meh. probably unavoidable, since
   // the mouse is moved only on the next frame, but it should
   // be possible to handle it more gracefully.
   bool should_wait_frame = false;

   switch (s_current_point)
   {
      case A:
         if (s_previous_point != A)
         {
            set_mouse_pos (c, s_point_a);
            should_wait_frame = true;

            s_previous_point = A;

            s_color_a = RED;
            s_color_b = BLACK;
            s_color_c = BLACK;
         }

         if (should_wait_frame)
            should_wait_frame = false;
         else
            s_point_a = mouse_pos;

         break;
      case B:
         if (s_previous_point != B)
         {
            set_mouse_pos (c, s_point_b);
            should_wait_frame = true;

            s_previous_point = B;

            s_color_a = BLACK;
            s_color_b = RED;
            s_color_c = BLACK;
         }

         if (should_wait_frame)
            should_wait_frame = false;
         else
            s_point_b = mouse_pos;

         break;
      case C:
         if (s_previous_point != C)
         {
            set_mouse_pos (c, s_point_c);
            should_wait_frame = true;

            s_previous_point = C;

            s_color_a = BLACK;
            s_color_b = BLACK;
            s_color_c = RED;
         }

         if (should_wait_frame)
            should_wait_frame = false;
         else
            s_point_c = mouse_pos;

         break;
   }

   // NOTE: flips sign depending on Y inversion
   s_cross_product_magnitude =
      (s_point_c.x - s_point_b.x) *
         (INVERT_Y (s_point_a.y) - INVERT_Y (s_point_b.y)) -
      (INVERT_Y (s_point_c.y) - INVERT_Y (s_point_b.y)) *
         (s_point_a.x - s_point_b.x);

   // NOTE: it's the same regardless of Y inversion
   s_dot_product = (s_point_c.x - s_point_b.x) * (s_point_a.x - s_point_b.x) +
                   (s_point_c.y - s_point_b.y) * (s_point_a.y - s_point_b.y);

   // NOTE: this way, one can get an unsigned angle
   float ab_length = sqrtf (powf (s_point_b.x - s_point_a.x, 2.) +
                            powf (s_point_b.y - s_point_a.y, 2.));
   float bc_length = sqrtf (powf (s_point_c.x - s_point_b.x, 2.) +
                            powf (s_point_c.y - s_point_b.y, 2.));

   // NOTE: this is an alternative angle acquirement method
   // float dot_product_norm =
   //   (s_point_b.x - s_point_a.x) / ab_length *
   //   (s_point_c.x - s_point_a.x) / ac_length -
   //   (s_point_b.y - s_point_a.y) / ab_length *
   //   (s_point_c.y - s_point_a.y) / ac_length;
   //
   // float cos_abc_norm = dot_product_norm;
   // float abc_angle = acosf(cos_abc_norm);

   // NOTE: flips ABC <-> CBA depending on Y inversion
   s_should_show_angle = ab_length != 0.f && bc_length != 0.f;
   s_abc_angle_signed  = atan2f (s_cross_product_magnitude, s_dot_product);
   s_abc_angle_360     = s_abc_angle_signed <= 0 ? 2 * PI + s_abc_angle_signed
                                                 : s_abc_angle_signed;
}

void
vector_products_scene_draw (Context* c)
{
   set_draw_font (&c->fonts->fixed_font);

   clear_buffer (rgb_from_rgba (LIGHTGRAY));

   draw_line (s_point_a, s_point_b, BLACK);
   draw_line (s_point_b, s_point_c, BLACK);
   draw_circle_f (s_point_a, 3.f, s_color_a);
   draw_circle_f (s_point_b, 3.f, s_color_b);
   draw_circle_f (s_point_c, 3.f, s_color_c);

   draw_textf (get_origin (c, s_point_a), 0, 0, "A1: %.f, A2: %.f",
               (double)s_point_a.x, (double)INVERT_Y (s_point_a.y));
   draw_textf (get_origin (c, s_point_b), 0, 0, "B1: %.f, B2: %.f",
               (double)s_point_b.x, (double)INVERT_Y (s_point_b.y));
   draw_textf (get_origin (c, s_point_c), 0, 0, "C1: %.f, C2: %.f",
               (double)s_point_c.x, (double)INVERT_Y (s_point_c.y));

   draw_text_center_i (120, 20, 200, 0, "cross-product magnitude");
   draw_text_center_i (120, 30, 200, 0, "(C1-B1)(A2-B2)-(C2-B2)(A1-B1)");
   draw_textf_center_i (
      120, 40, 200, 0, "(%*.f)(%*.f)-(%*.f)(%*.f) ", 3,
      (double)s_point_c.x - (double)s_point_b.x, 3,
      (double)INVERT_Y (s_point_a.y) - (double)INVERT_Y (s_point_b.y), 3,
      (double)INVERT_Y (s_point_c.y) - (double)INVERT_Y (s_point_b.y), 3,
      (double)s_point_a.x - (double)s_point_b.x);
   draw_textf_center_i (120, 50, 200, 0, "%*.f", 6,
                        (double)s_cross_product_magnitude);

   draw_text_center_i (120, 70, 200, 0, "dot product");
   draw_text_center_i (120, 80, 200, 0, "(C1-B1)(A1-B1)+(C2-B2)(A2-B2)");
   draw_textf_center_i (
      120, 90, 200, 0, "(%*.f)(%*.f)+(%*.f)(%*.f) ", 3,
      (double)s_point_c.x - (double)s_point_b.x, 3,
      (double)s_point_a.x - (double)s_point_b.x, 3,
      (double)INVERT_Y (s_point_c.y) - (double)INVERT_Y (s_point_b.y), 3,
      (double)INVERT_Y (s_point_a.y) - (double)INVERT_Y (s_point_b.y));
   draw_textf_center_i (120, 100, 200, 0, "%*.f", 6, (double)s_dot_product);

   draw_text_center_i (120, 120, 200, 0, "ABC angle");
   if (s_should_show_angle)
   {
      draw_textf_center_i (120, 130, 200, 0, "%*.f deg", 4,
                           (double)s_abc_angle_signed * 180 / (double)PI);
      draw_textf_center_i (120, 140, 200, 0, "%*.f deg", 3,
                           (double)s_abc_angle_360 * 180 / (double)PI);
   }
}

Vector2
get_origin (Context* c, Vector2 pos)
{
   float buffer_width = (float)c->pixel_buffer->image->width;
   return (Vector2){ .x = CLAMP (pos.x - 40, 0, buffer_width - 1),
                     .y = pos.y - 10 };
}
