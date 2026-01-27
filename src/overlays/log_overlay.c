#include "log_overlay.h"

#include "../config.h"
#include "../context.h"
#include "../draw_utils.h"
#include "../log.h"

#include <raylib.h>
#include <raymath.h>

#define LINES_PER_PAGE     21
#define LINE_INDEX_COUNT   4096
#define LOG_LEFT_PADDING   5
#define LOG_TOP_PADDING    3
#define LOG_BOTTOM_PADDING 4

static Image*  s_overlay;
static char*   s_logs[LOG_COUNT];
static Indices s_log_indices;
static size_t  s_current_log_page;
static size_t  s_last_log_page;
static LogIdx  s_selected_log = INTERNAL_LOG;

bool
log_overlay_init (Context* c)
{
   s_overlay = clone_image (c->pixel_buffer->image);
   if (s_overlay == NULL)
      ERROR_RETURN (false, "clone image");

   for (size_t i = 0; i < LOG_COUNT; ++i)
   {
      s_logs[i] = calloc (LOG_BUFFER_SIZE, sizeof (char));
      if (s_logs[i] == NULL)
         ERROR_RETURN (false, "log %d calloc", i);

      log_copy (s_logs[i], (LogIdx)i);
   }

   s_log_indices.data = calloc (LINE_INDEX_COUNT, sizeof (size_t));
   if (s_log_indices.data == NULL)
      ERROR_RETURN (false, "log indices data calloc");

   copy_text_line_indices_i (&s_log_indices, LOG_LEFT_PADDING,
                             c->pixel_buffer->image->width,
                             s_logs[s_selected_log]);

   s_last_log_page    = s_log_indices.count / LINES_PER_PAGE;
   s_current_log_page = s_last_log_page;

   return true;
}

bool
log_overlay_deinit (Context* c)
{
   indices_free (s_log_indices);
   for (size_t i = 0; i < LOG_COUNT; ++i)
      free (s_logs[i]);

   UnloadImage (*s_overlay);
   free (s_overlay);

   return true;
}

void
log_overlay_update (Context* c, float dt)
{
   if (!c->state->should_show_log)
      return;

   for (size_t i = 0; i < LOG_COUNT; ++i)
      log_copy (s_logs[i], (LogIdx)i);

   copy_text_line_indices_i (&s_log_indices, LOG_LEFT_PADDING,
                             c->pixel_buffer->image->width,
                             s_logs[s_selected_log]);

   s_last_log_page = s_log_indices.count / LINES_PER_PAGE;

   if (c->input->log_page_up)
   {
      previous_log_page ();
   }
   if (c->input->log_page_down)
   {
      next_log_page ();
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

   const size_t top_line_index = s_last_log_page == 0 ? 0
                               : s_current_log_page == s_last_log_page
                                  ? s_log_indices.count - LINES_PER_PAGE - 1
                                  : s_current_log_page * LINES_PER_PAGE;

   draw_text_i (LOG_LEFT_PADDING, LOG_TOP_PADDING,
                c->pixel_buffer->image->width,
                c->pixel_buffer->image->height - LOG_BOTTOM_PADDING,
                &s_logs[s_selected_log][s_log_indices.data[top_line_index]]);
}

void
previous_log_page (void)
{
   if (s_current_log_page == 0)
      return;

   s_current_log_page -= 1;
}

void
next_log_page (void)
{
   if (s_current_log_page == s_last_log_page)
      return;

   s_current_log_page += 1;
}
