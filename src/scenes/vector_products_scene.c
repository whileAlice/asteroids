#include <raylib.h>
#include <raymath.h>

#include "vector_products_scene.h"
#include "../config.h"
#include "../context.h"
#include "../math_utils.h"
#include "../draw_utils.h"
#include "../window_utils.h"
#include "../log.h"

// TODO: handle Y axis inversion in a clearer fashion
#define INVERT_Y(y) ((float)(PIXEL_BUFFER_HEIGHT - 1) - y)

typedef enum selected_point{
  A, B, C
} SelectedPoint;

static Vector2       s_point_a, s_point_b, s_point_c;
static Color         s_color_a, s_color_b, s_color_c;
static SelectedPoint s_current_point, s_previous_point;
static size_t        s_mouse_update_count;

void
vector_products_scene_init(Context* c)
{
  s_point_a = (Vector2){ .x = 23.f,  .y = 40.f  };
  s_point_b = (Vector2){ .x = 61.f,  .y = 171.f };
  s_point_c = (Vector2){ .x = 112.f, .y = 65.f  };

  s_color_a = RED;
  s_color_b = BLACK;
  s_color_c = BLACK;

  s_current_point = A;
  s_previous_point = A;

  s_mouse_update_count = 2;
}

void vector_products_scene_deinit(){}

void
vector_products_scene_update(Context* c, float dt)
{
  // TODO: not cool. could raylib be told where to put
  // the mouse at init in a more robust manner?
  if (s_mouse_update_count != 0) {
    set_mouse_pos(c, s_point_a);
    s_mouse_update_count--;
  }

  if (IsKeyPressed(KEY_SPACE)) {
    switch (s_current_point) {
    case A:
      s_current_point = B;
      break;
    case B:
      s_current_point = C;
      break;
    case C:
      s_current_point = A;
      break;
    }
  }

  if (IsKeyPressed(KEY_ONE)) {
    s_current_point = A;
  }
  if (IsKeyPressed(KEY_TWO)) {
    s_current_point = B;
  }
  if (IsKeyPressed(KEY_THREE)) {
    s_current_point = C;
  }

  int mouse_x = get_mouse_x(c);
  int mouse_y = get_mouse_y(c);
  Vector2 mouse_pos = { .x = (float)mouse_x, .y = (float)mouse_y };

  // TODO: this is also kinda meh. probably unavoidable, since
  // the mouse is moved only on the next frame, but it should
  // be possible to handle it more gracefully.
  bool should_wait_frame = false;

  switch (s_current_point) {
  case A:
    if (s_previous_point != A) {
      set_mouse_pos(c, s_point_a);
      should_wait_frame = true;

      s_previous_point = A;

      s_color_a = RED;
      s_color_b = BLACK;
      s_color_c = BLACK;
    }

    if (should_wait_frame) {
      should_wait_frame = false;
    } else {
      s_point_a = mouse_pos;
    }

    break;
  case B:
    if (s_previous_point != B) {
      set_mouse_pos(c, s_point_b);
      should_wait_frame = true;

      s_previous_point = B;

      s_color_a = BLACK;
      s_color_b = RED;
      s_color_c = BLACK;
    }

    if (should_wait_frame) {
      should_wait_frame = false;
    } else {
      s_point_b = mouse_pos;
    }

    break;
  case C:
    if (s_previous_point != C) {
      set_mouse_pos(c, s_point_c);
      should_wait_frame = true;

      s_previous_point = C;

      s_color_a = BLACK;
      s_color_b = BLACK;
      s_color_c = RED;
    }

    if (should_wait_frame) {
      should_wait_frame = false;
    } else {
      s_point_c = mouse_pos;
    }

    break;
  }

  // NOTE: flips sign depending on Y inversion
  float cross_product_magnitude =
    (s_point_c.x - s_point_b.x) *
    (INVERT_Y(s_point_a.y) - INVERT_Y(s_point_b.y)) -
    (INVERT_Y(s_point_c.y) - INVERT_Y(s_point_b.y)) *
    (s_point_a.x - s_point_b.x);

  // NOTE: it's the same regardless of Y inversion
  float dot_product =
    (s_point_c.x - s_point_b.x) * (s_point_a.x - s_point_b.x) +
    (s_point_c.y - s_point_b.y) * (s_point_a.y - s_point_b.y);

  // NOTE: this way, one can get an unsigned angle
  float ab_length = sqrtf(powf(s_point_b.x - s_point_a.x, 2.) +
                          powf(s_point_b.y - s_point_a.y, 2.));
  float bc_length = sqrtf(powf(s_point_c.x - s_point_b.x, 2.) +
                          powf(s_point_c.y - s_point_b.y, 2.));

  // float dot_product_norm =
  //   (s_point_b.x - s_point_a.x) / ab_length *
  //   (s_point_c.x - s_point_a.x) / ac_length -
  //   (s_point_b.y - s_point_a.y) / ab_length *
  //   (s_point_c.y - s_point_a.y) / ac_length;
  //
  // float cos_abc_norm = dot_product_norm;
  // float abc_angle = acosf(cos_abc_norm);

  // NOTE: flips ABC <-> CBA depending on Y inversion
  float abc_angle_signed = atan2f(cross_product_magnitude, dot_product);
  float abc_angle_360 = abc_angle_signed <= 0 ?
                                              2*PI + abc_angle_signed :
                                              abc_angle_signed;

  osd_print (2, 25, "cross-product magnitude");
  osd_print (3, 22, "(C1-B1)(A2-B2)-(C2-B2)(A1-B1)");
  osd_printf(4, 26,
             "(%*.f)(%*.f)-(%*.f)(%*.f) ",
             3, (double)s_point_c.x   - (double)s_point_b.x,
             3, (double)INVERT_Y(s_point_a.y) - (double)INVERT_Y(s_point_b.y),
             3, (double)INVERT_Y(s_point_c.y) - (double)INVERT_Y(s_point_b.y),
             3, (double)s_point_a.x   - (double)s_point_b.x);
  osd_printf(6, 33, "%*.f", 6, (double)cross_product_magnitude);

  osd_print (8,  31, "dot product");
  osd_print (9,  22, "(C1-B1)(A1-B1)+(C2-B2)(A2-B2)");
  osd_printf(10, 26,
             "(%*.f)(%*.f)+(%*.f)(%*.f) ",
             3, (double)s_point_c.x   - (double)s_point_b.x,
             3, (double)s_point_a.x   - (double)s_point_b.x,
             3, (double)INVERT_Y(s_point_c.y) - (double)INVERT_Y(s_point_b.y),
             3, (double)INVERT_Y(s_point_a.y) - (double)INVERT_Y(s_point_b.y));
  osd_printf(12, 33, "%*.f", 6, (double)dot_product);

  osd_print (14, 32, "ABC angle");
  if (ab_length != 0.f && bc_length != 0.f) {
    osd_printf(16, 32, "%*.f deg",
               4, (double)abc_angle_signed * 180 / (double)PI);
    osd_printf(17, 33, "%*.f deg",
               3, (double)abc_angle_360    * 180 / (double)PI);
  }
}

void
vector_products_scene_draw(Context* c, Image* buf)
{
  clear_rgb_image(buf, rgb_from_rgba(LIGHTGRAY));

  draw_line(buf, s_point_a, s_point_b, BLACK);
  draw_line(buf, s_point_b, s_point_c, BLACK);
  draw_circle_f(buf, s_point_a, 3.f, s_color_a);
  draw_circle_f(buf, s_point_b, 3.f, s_color_b);
  draw_circle_f(buf, s_point_c, 3.f, s_color_c);

  draw_textf(buf, &c->font.fixed_font,
             (Vector2){ .x = CLAMP(s_point_a.x - 40, 0,
                                   (float)c->buffer.image.width - 1),
                        .y = s_point_a.y - 10 },
              "A1: %.f, A2: %.f",
             (double)s_point_a.x,
             (double)INVERT_Y(s_point_a.y));
  draw_textf(buf, &c->font.fixed_font,
             (Vector2){ .x = CLAMP(s_point_b.x - 40, 0,
                                   (float)c->buffer.image.width - 1),
                        .y = s_point_b.y - 10 },
             "B1: %.f, B2: %.f",
             (double)s_point_b.x,
             (double)INVERT_Y(s_point_b.y));
  draw_textf(buf, &c->font.fixed_font,
             (Vector2){ .x = CLAMP(s_point_c.x - 40, 0,
                                   (float)c->buffer.image.width - 1),
                        .y = s_point_c.y - 10 },
             "C1: %.f, C2: %.f",
             (double)s_point_c.x,
             (double)INVERT_Y(s_point_c.y));
}
