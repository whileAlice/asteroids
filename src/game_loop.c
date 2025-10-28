#include <raylib.h>

#include "config.h"
#include "context.h"
#include "game_loop.h"
#include "input.h"
#include "draw_utils.h"
#include "log.h"

extern Context g_ctx;

static Image overlay;

void
game_init()
{
  g_ctx.state.show_osd = true;
  load_fixed_font(FIXED_FONT_PATH);
  init_log_buffers();
  log_printf("loaded fixed font with %d glyphs", g_ctx.fixed_font.glyph_count);

  overlay = clone_image(g_ctx.buffer_image);
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

  osd_printf(0, 0, "fps: %.1f", 1. / dt);
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
  // draw_image_a(buf, &glyph_sheet, mouse_x, mouse_y);
  if (g_ctx.state.show_osd) {
    draw_osd(buf);
  }
  if (g_ctx.state.show_log) {
    box_blur(&overlay, buf);
    draw_image(buf, &overlay, 0, 0);
    draw_log(buf);
  }
}

void
game_deinit()
{
  UnloadImage(overlay);
  deinit_log_buffers();
  unload_fixed_font();
}
