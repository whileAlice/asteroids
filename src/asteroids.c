#include "buffer.h"
#include "config.h"
#include "context.h"
#include "draw_utils.h"
#include "error.h"
#include "event.h"
#include "game_loop.h"
#include "log.h"
#include "threads.h"
#include "ui_layer_manager.h"
#include "window_utils.h"

#include <raylib.h>

int
main (void)
{
   if (!log_init ())
      ERROR_GOTO (end, "log init");

   Context* c = context_create ();
   if (c == NULL)
      ERROR_GOTO (end, "context create");

   PixelBuffer* pb = c->pixel_buffer;
   Fonts*       f  = c->fonts;
   Event*       e  = c->event;

   if (!threads_init (c))
      ERROR_GOTO (end, "threads init");

   log_info ("Initializing game...");

   SetTraceLogCallback (&raylib_tracelog_callback);

   InitWindow (INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, APP_TITLE);

   SetExitKey (EXIT_KEY);
   SetTargetFPS (TARGET_FPS);
   HideCursor ();

   Image* image = buffer_image_create ();
   if (image == NULL)
      ERROR_GOTO (end, "buffer image generate");

   pb->image   = image;
   pb->texture = LoadTextureFromImage (*pb->image);
   if (!load_fixed_fonts (&f->fixed_font, &f->fixed_font_inverted,
                          FIXED_FONT_PATH))
      ERROR_GOTO (end, "load fixed fonts");

   set_draw_font (&f->fixed_font);
   set_draw_buffer (pb->image);
   set_buffer_scale_and_texture_origin (c);

   if (!active_modals_init ())
      ERROR_GOTO (end, "active modals init");

   if (!active_overlays_init ())
      ERROR_GOTO (end, "active overlays init");

   while (!should_quit_app (c))
   {
      if (!game_init (c))
         ERROR_GOTO (end, "game init");

      while (!e->should_deinit_game)
      {
         while (!e->has_event && !should_close_window (c))
         {
            if (IsWindowResized ())
               set_buffer_scale_and_texture_origin (c);

            float dt = GetFrameTime ();

            game_update (c, dt);
            game_draw (c);

            UpdateTexture (pb->texture, pb->image->data);

            BeginDrawing ();

            ClearBackground (BLACK);
            DrawTextureEx (pb->texture,
                           (Vector2){ .x = (float)pb->texture_origin_x,
                                      .y = (float)pb->texture_origin_y },
                           .0, (float)pb->integer_scale, WHITE);

            EndDrawing ();
         }

         if (!event_handle (c))
            ERROR_GOTO (end, "event handle");
      }

      if (!game_deinit (c))
         ERROR_GOTO (end, "game deinit");
   }

   unload_fixed_font_images ();
   UnloadTexture (pb->texture);
   image_free (image);

   CloseWindow ();

   if (!threads_deinit (c))
      ERROR_GOTO (end, "threads deinit");

   context_free (c);

end:
   log_debug ("Returning...");

   if (has_error (UNKNOWN_THREAD))
   {
      log_error ("Unknown thread error below:");
      error_print (UNKNOWN_THREAD);
   }

   ThreadIdx thread_idx = get_thread_idx (pthread_self ());
   if (has_error (thread_idx))
   {
      error_print (thread_idx);
      return EXIT_FAILURE;
   }

   log_deinit ();

   return EXIT_SUCCESS;
}
