#include "osd_overlay.h"

#include "../context.h"
#include "../draw_utils.h"

#include <raylib.h>
#include <raymath.h>

double s_fps;

bool
osd_overlay_init (Context* c)
{
   return true;
}

bool
osd_overlay_deinit (Context* c)
{
   return true;
}

void
osd_overlay_update (Context* c, float dt)
{
   if (!c->state->should_show_osd)
      return;

   s_fps = 1. / (double)dt;
}

void
osd_overlay_draw (Context* c)
{
   if (!c->state->should_show_osd)
      return;

   FixedFont* font = c->state->has_overlay ? &c->fonts->fixed_font_inverted
                                           : &c->fonts->fixed_font;
   set_draw_font (font);

   draw_textf_i (10, 10, "fps: %*.1f", 4, s_fps);
}
