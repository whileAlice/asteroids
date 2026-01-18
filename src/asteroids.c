#include "buffer.h"
#include "config.h"
#include "context.h"
#include "error.h"
#include "event.h"
#include "game_loop.h"
#include "log.h"
#include "threads.h"
#include "window_utils.h"

#include <fcntl.h>
#include <pthread.h>
#include <raylib.h>
#include <stdlib.h>
#include <unistd.h>

int
main (void)
{
   int          err;
   ssize_t      length;
   Context*     c  = context_create ();
   Log*         l  = c->log;
   PixelBuffer* pb = c->pixel_buffer;
   Fonts*       f  = c->fonts;
   Event*       e  = c->event;

   err = pipe (l->wakeup_pipe);
   if (err == -1)
      ERRNO_GOTO (end, "wakeup pipe");

   for (size_t i = 0; i <= WRITE_END; ++i)
   {
      int flags = fcntl (l->wakeup_pipe[i], F_GETFL);
      if (flags == -1)
         ERRNO_GOTO (end, "wakeup pipe[%zu] F_GETFL", i);

      err = fcntl (l->wakeup_pipe[i], F_SETFL, flags | O_NONBLOCK);
      if (err == -1)
         ERRNO_GOTO (end, "wakeup pipe[%zu] O_NONBLOCK", i);
   }

   if (!threads_init (c))
      ERROR_GOTO (end, "threads init");

   log_debug ("syncing...");
   SYNC_THREAD (&l->mutex, &l->cond, l->thread_ready_count,
                STREAMER_THREAD_COUNT, l->should_abort_init, end);
   log_debug ("ready!");

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
   if (!load_fixed_fonts (&f->fixed_font, &f->inverted_fixed_font,
                          FIXED_FONT_PATH))
      ERROR_GOTO (end, "load fixed fonts");

   set_buffer_scale_and_texture_origin (c);

   while (!should_quit_app (c))
   {

      if (!game_init (c))
         ERROR_GOTO (end, "game init");

      while (!e->should_deinit_game)
      {
         while (!e->has_event)
         {
            if (IsWindowResized ())
               set_buffer_scale_and_texture_origin (c);

            float dt = GetFrameTime ();

            game_update (c, dt);
            game_draw (c, pb->image);

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
   buffer_image_free (image);

   CloseWindow ();

   const char wakeup = '\0';
   length = write (l->wakeup_pipe[WRITE_END], &wakeup, sizeof (char));
   if (length == -1)
      ERRNO_GOTO (end, "wakeup pipe write");

   if (!threads_deinit (c))
      ERROR_GOTO (end, "threads deinit");

   context_free (c);

end:
   log_debug ("returning...");

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

   return EXIT_SUCCESS;
}
