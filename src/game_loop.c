#include <raylib.h>

#include "game_loop.h"
#include "window_utils.h"
#include "draw_utils.h"
#include "pam.h"
#include "log.h"

static int       mouse_x;
static int       mouse_y;
static Image     glyph_sheet;
static FixedFont font;

void
game_init()
{
  glyph_sheet = image_from_pam("assets/4x8font.pam");
  font = load_fixed_font(&glyph_sheet,
                         (Padding){
                           .top    = 1,
                           .bottom = 0,
                           .left =   1,
                           .right =  1,
                         },
                         4, 8, 2, 52);
  init_log_buffers(&font);
  add_line_to_console_log("HELLO, WORLD");
}

void
game_update(float dt)
{
  mouse_x = get_mouse_x();
  mouse_y = get_mouse_y();
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
  draw_text(buf, &font, mouse_x, mouse_y, "Y HALO THAR one two three", 1);
  // draw_image_a(buf, &glyph_sheet, mouse_x, mouse_y);
  show_console_log(buf);
}

void
game_deinit()
{
  deinit_log_buffers();
  UnloadImage(glyph_sheet);
}
