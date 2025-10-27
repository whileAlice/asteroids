#include <raylib.h>

#include "game_loop.h"
#include "window_utils.h"
#include "draw_utils.h"
#include "log.h"

extern FixedFont g_fixed_font;

static int  s_mouse_x;
static int  s_mouse_y;
static char s_mouse_x_str[32];

void
game_init()
{
  load_fixed_font("assets/5x8font.def");
  init_log_buffers();
  add_line_to_console_log("Hello, world!");
}

void
game_update(float dt)
{
  s_mouse_x = get_mouse_x();
  s_mouse_y = get_mouse_y();

  sprintf(s_mouse_x_str, "%d", s_mouse_x);
  print_to_osd(s_mouse_x_str, 0, 0);
}

void
game_draw(Image* buf)
{
  clear_image_rgb(buf, rgb_from_rgba(GRAY));
  draw_triangle(buf,
                (Vector2){ .x = 50.0f,  .y = 100.0f },
                (Vector2){ .x = 100.0f, .y = 40.0f },
                (Vector2){ .x = 200.0f, .y = 180.0f },
                RED, GREEN);
  draw_rectangle_fi(buf, 20, 20, 50, 50, BLUE);
  draw_text(buf, &g_fixed_font, s_mouse_x, s_mouse_y,
            "Y HALO THAR one two three", 1);
  // draw_image_a(buf, &glyph_sheet, mouse_x, mouse_y);
  draw_console_log(buf);
  draw_osd(buf);
}

void
game_deinit()
{
  deinit_log_buffers();
  unload_fixed_font();
}
