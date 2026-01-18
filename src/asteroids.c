#include "buffer.h"
#include "config.h"
#include "context.h"
#include "error.h"
#include "game_loop.h"
#include "log.h"
#include "threads.h"
#include "window_loop.h"

#include <fcntl.h>
#include <pthread.h>
#include <raylib.h>
#include <stdlib.h>
#include <unistd.h>

int
main (void)
{
   int      err;
   ssize_t  length;
   Context* c = context_create ();
   Log*     l = c->log;

   err = pipe (c->log->wakeup_pipe);
   if (err == -1)
      ERRNO_RETURN (EXIT_FAILURE, "wakeup pipe");

   for (size_t i = 0; i <= WRITE_END; ++i)
   {
      int flags = fcntl (c->log->wakeup_pipe[i], F_GETFL);
      if (flags == -1)
         ERRNO_RETURN (EXIT_FAILURE, "wakeup pipe[%zu] F_GETFL", i);

      err = fcntl (c->log->wakeup_pipe[i], F_SETFL, flags | O_NONBLOCK);
      if (err == -1)
         ERRNO_RETURN (EXIT_FAILURE, "wakeup pipe[%zu] O_NONBLOCK", i);
   }

   if (!threads_init (c))
      ERROR_RETURN (EXIT_FAILURE, "threads init");

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
      ERROR_RETURN (EXIT_FAILURE, "buffer image generate");

   c->pixel_buffer->image   = image;
   c->pixel_buffer->texture = LoadTextureFromImage (*c->pixel_buffer->image);

   if (!game_init (c))
      ERROR_RETURN (EXIT_FAILURE, "game init");

   window_loop (c);

   if (!game_deinit (c))
      ERROR_RETURN (EXIT_FAILURE, "game deinit");

   UnloadTexture (c->pixel_buffer->texture);
   buffer_image_free (image);

   CloseWindow ();

   IN_LOCK(&c->app->mutex,
      c->app->should_quit = true;
   );

   const char wakeup = '\0';
   length = write (c->log->wakeup_pipe[WRITE_END], &wakeup, sizeof (char));
   if (length == -1)
      ERRNO_RETURN (EXIT_FAILURE, "wakeup pipe write");

end:
   log_debug ("returning...");

   if (!threads_deinit (c))
      ERROR_RETURN (EXIT_FAILURE, "threads deinit");

   context_free (c);

   return EXIT_SUCCESS;
}
