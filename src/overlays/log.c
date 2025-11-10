#include <raylib.h>
#include <raymath.h>

#include "log.h"
#include "../ui_layer.h"
#include "../context.h"
#include "../log.h"
#include "../draw_utils.h"

static Image s_overlay;

void
log_overlay_init(UILayer* self, Context* c)
{
  log_printf("loaded fixed font with %d glyphs", c->font.fixed_font.glyph_count);
  log_printf("loaded inverted fixed font with %d glyphs",
             c->font.inverted_fixed_font.glyph_count);

  s_overlay = clone_image(&c->buffer.image);
}

void
log_overlay_deinit(UILayer* self)
{
  self->is_visible = false;
  UnloadImage(s_overlay);
}

void
log_overlay_update(UILayer* self, Context* c, float dt)
{
  // TODO: this should be somewhere else?
  // maybe set is_visible directly somehow by the input?
  if (c->state.show_log) {
    self->is_visible = true;
  }

  if (!self->is_visible) return;

  if (c->input.log_page_up) {
    previous_log_page(c);
  }
  if (c->input.log_page_down) {
    next_log_page(c);
  }
}

void
log_overlay_draw(UILayer* self, Context* c, Image* buf)
{
  if (!self->is_visible) return;

  box_blur(&s_overlay, buf, 1);
  brighten_image_by_percentage(&s_overlay, &s_overlay, 20);
  draw_rgb_image(buf, &s_overlay, Vector2Zero());
  draw_log(c, buf, true);
}
