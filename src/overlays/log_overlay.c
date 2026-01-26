#include "log_overlay.h"

#include "../context.h"
#include "../draw_utils.h"
#include "../log.h"

#include <raylib.h>
#include <raymath.h>

static Image* s_overlay;
static char*  s_logs[LOG_COUNT];
static size_t s_log_indices[LOG_COUNT];

bool
log_overlay_init (Context* c)
{
   s_overlay = clone_image (c->pixel_buffer->image);
   if (s_overlay == NULL)
      ERROR_RETURN (false, "clone image");

   return true;
}

bool
log_overlay_deinit (Context* c)
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

   // TODO: switch to LogViews
   for (size_t i = 0; i < LOG_COUNT; ++i)
      s_logs[i] = get_log_copy ((LogIdx)i);

   if (c->input->log_page_up)
   {
      // previous_log_page (c);
   }
   if (c->input->log_page_down)
   {
      // next_log_page (c);
   }
}

// TODO: colorful logs!
void
log_overlay_draw (Context* c)
{
   if (!c->state->should_show_log)
      return;

   set_draw_font (&c->fonts->fixed_font_inverted);

   box_blur (s_overlay, c->pixel_buffer->image, 1);
   brighten_image_by_percentage (s_overlay, s_overlay, 20);
   draw_rgb_overlay (s_overlay);

   size_t last =
      draw_text_i (0, 0, c->pixel_buffer->image->width - 5, 100, s_logs[0]);
   size_t last2 =
      draw_text_i (0, 108, c->pixel_buffer->image->width - 5, 100, &s_logs[0][last]);

   for (size_t i = 0; i < LOG_COUNT; ++i)
      free (s_logs[i]);
}
