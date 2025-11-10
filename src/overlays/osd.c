#include <raylib.h>
#include <raymath.h>

#include "osd.h"
#include "../ui_layer.h"
#include "../context.h"
#include "../log.h"

void
osd_overlay_init(UILayer* self, Context* c)
{
}

void
osd_overlay_deinit(UILayer* self)
{
  self->is_visible = false;
}

void
osd_overlay_update(UILayer* self, Context* c, float dt)
{
  if (!self->is_visible) return;

  osd_printf(0, 0, "fps: %*.1f", 4, 1. / (double)dt);
}

void
osd_overlay_draw(UILayer* self, Context* c, Image* buf)
{
  if (!self->is_visible) return;

  bool is_inverted = c->state.show_log;
  draw_osd(c, buf, is_inverted);
}
