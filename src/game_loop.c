#include <raylib.h>

#include "config.h"
#include "context.h"
#include "game_loop.h"
#include "input.h"
#include "window_utils.h"
#include "draw_utils.h"
#include "log.h"

extern Context g_ctx;

static int  s_mouse_x;
static int  s_mouse_y;

void
game_init()
{
  load_fixed_font(FIXED_FONT_PATH);
  init_log_buffers();
  log_printf("loaded fixed font with %d glyphs", g_ctx.fixed_font.glyph_count);
}

void
game_update(float dt)
{
  update_input();
  update_state();

  if (g_ctx.input.log_page_up) {
    previous_log_page();
  }
  if (g_ctx.input.log_page_down) {
    next_log_page();
  }
  if (IsKeyPressed(KEY_L)) {
    log_print("THE L KEY IS PRESSED");
  }

  s_mouse_x = get_mouse_x();
  s_mouse_y = get_mouse_y();

  osd_printf(0, 0, "s_mouse_x: %d, s_mouse_y: %d", s_mouse_x, s_mouse_y);
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
  draw_text(buf, &g_ctx.fixed_font, s_mouse_x, s_mouse_y,
            "Y HALO THAR one two three", 1);
  // draw_image_a(buf, &glyph_sheet, mouse_x, mouse_y);
  if (g_ctx.state.show_log) {
    draw_log(buf);
  }
  if (g_ctx.state.show_osd) {
    draw_osd(buf);
  }
}

void
game_deinit()
{
  deinit_log_buffers();
  unload_fixed_font();
}
