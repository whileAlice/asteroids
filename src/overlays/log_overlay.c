#include "log_overlay.h"

#include "../context.h"
#include "../draw_utils.h"

#include <raylib.h>
#include <raymath.h>

static Image* s_overlay;

bool
log_overlay_init (Context* c)
{
   // log_printf ("loaded fixed font with %d glyphs",
   //             c->fonts->fixed_font.glyph_count);
   // log_printf ("loaded inverted fixed font with %d glyphs",
   //             c->fonts->inverted_fixed_font.glyph_count);

   s_overlay = clone_image (c->pixel_buffer->image);
   if (s_overlay == NULL)
      ERROR_RETURN (false, "clone image");

   return true;
}

bool
log_overlay_deinit (void)
{
   UnloadImage (*s_overlay);
   free (s_overlay);

   return true;
}

void
log_overlay_update (Context* c, float dt)
{
   if (!c->state->should_show_log)
      return;

   if (c->input->log_page_up)
   {
      // previous_log_page (c);
   }
   if (c->input->log_page_down)
   {
      // next_log_page (c);
   }
}

void
log_overlay_draw (Context* c, Image* buf)
{
   if (!c->state->should_show_log)
      return;

   box_blur (s_overlay, buf, 1);
   brighten_image_by_percentage (s_overlay, s_overlay, 20);
   draw_rgb_image (buf, s_overlay, Vector2Zero ());
   // draw_log (c, buf, true);
}
