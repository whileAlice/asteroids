#include "osd_overlay.h"

#include "../context.h"
#include "../ui_layer.h"

#include <raylib.h>
#include <raymath.h>

bool
osd_overlay_init (UILayer* self, Context* c)
{
   return true;
}

bool
osd_overlay_deinit (UILayer* self)
{
   self->is_visible = false;
   return true;
}

void
osd_overlay_update (UILayer* self, Context* c, float dt)
{
   if (!self->is_visible)
      return;

   // osd_printf (0, 0, "fps: %*.1f", 4, 1. / (double)dt);
}

void
osd_overlay_draw (UILayer* self, Context* c, Image* buf)
{
   if (!self->is_visible)
      return;

   // bool is_inverted = c->state->should_show_log;
   // draw_osd (c, buf, is_inverted);
}
