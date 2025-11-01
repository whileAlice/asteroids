#include <raylib.h>

#include "config.h"
#include "context.h"
#include "game_loop.h"
#include "draw_utils.h"
#include "log.h"
#include "update_state.h"
#include "update_input.h"

static Image overlay;

void
game_init(Context* c)
{
  c->state.show_osd = true;
  load_fixed_fonts(&c->fixed_font, &c->fixed_font_inverted, FIXED_FONT_PATH);
  init_log_buffers(c);
  log_printf("loaded fixed font with %d glyphs", c->fixed_font.glyph_count);

  overlay = clone_image(&c->buffer_image);
}

void
game_update(Context* c, float dt)
{
  update_input(c);
  update_state(c);

  if (c->input.log_page_up) {
    previous_log_page(c);
  }
  if (c->input.log_page_down) {
    next_log_page(c);
  }
  if (IsKeyPressed(KEY_L)) {
    log_print("THE L KEY IS PRESSED");
  }

  osd_printf(0, 0, "fps: %.1f", 1. / (double)dt);
}

void
game_draw(Context* c, Image* buf)
{
  clear_rgb_image(buf, rgb_from_rgba(PINK));
  draw_triangle(buf,
                (Vector2){ .x = 50.0f,  .y = 100.0f },
                (Vector2){ .x = 100.0f, .y = 40.0f },
                (Vector2){ .x = 200.0f, .y = 180.0f },
                MAROON, DARKPURPLE);
  draw_rectangle_fi(buf, 20, 20, 50, 50, BLUE);
  if (c->state.show_log) {
    box_blur(&overlay, buf, 1);
    brighten_image_by_percentage(&overlay, &overlay, 20);
    draw_rgb_image(buf, &overlay, 0, 0);
    draw_log(c, buf, true);
  }
  if (c->state.show_osd) {
    draw_osd(c, buf, c->state.show_log);
  }
}

void
game_deinit()
{
  UnloadImage(overlay);
  deinit_log_buffers();
  unload_fixed_font_images();
}
