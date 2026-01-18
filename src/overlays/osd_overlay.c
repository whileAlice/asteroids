#include "osd_overlay.h"

#include "../context.h"

#include <raylib.h>
#include <raymath.h>

bool
osd_overlay_init (Context* c)
{
   return true;
}

bool
osd_overlay_deinit (void)
{
   return true;
}

void
osd_overlay_update (Context* c, float dt)
{
   if (!c->state->should_show_log)
      return;

   // osd_printf (0, 0, "fps: %*.1f", 4, 1. / (double)dt);
}

void
osd_overlay_draw (Context* c, Image* buf)
{
   if (!c->state->should_show_log)
      return;

   // bool is_inverted = c->state->should_show_log;
   // draw_osd (c, buf, is_inverted);
}
