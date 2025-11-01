
#include <raylib.h>

#include "log_display.h"
#include "../config.h"
#include "../context.h"
#include "../log.h"
#include "../draw_utils.h"


static Image s_overlay;

void
log_display_init(Context* c)
{
  c->state.show_osd = true;

  load_fixed_fonts(&c->fixed_font, &c->fixed_font_inverted, FIXED_FONT_PATH);
  init_log_buffers(c);

  log_printf("loaded fixed font with %d glyphs", c->fixed_font.glyph_count);
  log_printf("loaded inverted fixed font with %d glyphs",
             c->fixed_font_inverted.glyph_count);

  s_overlay = clone_image(&c->buffer_image);
}

void
log_display_update(Context* c, float dt)
{
  if (c->input.log_page_up) {
    previous_log_page(c);
  }
  if (c->input.log_page_down) {
    next_log_page(c);
  }

  osd_printf(0, 0, "fps: %.1f", 1. / (double)dt);
}

void
log_display_draw(Context* c, Image* buf)
{
  if (c->state.show_log) {
    box_blur(&s_overlay, buf, 1);
    brighten_image_by_percentage(&s_overlay, &s_overlay, 20);
    draw_rgb_image(buf, &s_overlay, 0, 0);
    draw_log(c, buf, true);
  }
  if (c->state.show_osd) {
    draw_osd(c, buf, c->state.show_log);
  }
}

void
log_display_deinit()
{
  UnloadImage(s_overlay);
  deinit_log_buffers();
  unload_fixed_font_images();
}
