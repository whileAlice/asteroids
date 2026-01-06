#include "buffer.h"
#include "config.h"
#include "context.h"
#include "game_loop.h"
#include "log.h"
#include "threads.h"
#include "window_loop.h"

#include <pthread.h>
#include <raylib.h>
#include <stdlib.h>
#include <unistd.h>

#define FAIL(msg)          \
   do                      \
   {                       \
      perror (msg);        \
      return EXIT_FAILURE; \
   }                       \
   while (0)

int
main (void)
{
   int      err;
   ssize_t  length;
   Context* c = context_create ();

   Log* l = c->log;

   err = pipe (c->log->wakeup_pipe);
   if (err == -1)
      FAIL ("wakeup pipe");

   err = threads_init (c);
   if (err != 0)
      FAIL ("threads_init");

   debug ("main syncing...");
   SYNC_THREAD (&l->mutex, &l->cond, l->thread_ready_count, LOG_THREAD_COUNT,
                l->should_abort_init, end);
   debug ("main ready!");

   SetTraceLogCallback(&raylib_tracelog_callback);

   InitWindow (INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, APP_TITLE);

   SetExitKey (EXIT_KEY);
   SetTargetFPS (TARGET_FPS);
   HideCursor ();

   c->pixel_buffer->image   = generate_buffer_image ();
   c->pixel_buffer->texture = LoadTextureFromImage (c->pixel_buffer->image);

   game_init (c);
   window_loop (c);
   game_deinit (c);

   UnloadTexture (c->pixel_buffer->texture);
   UnloadImage (c->pixel_buffer->image);

   CloseWindow ();

   IN_LOCK(&c->state->mutex,
      c->state->should_exit_app = true;
   );

   const char wakeup = '\0';
   length = write (c->log->wakeup_pipe[WRITE_END], &wakeup, sizeof (char));
   if (length <= 0)
      FAIL ("wakeup pipe write");

end:
   debug ("main returning...");

   err = threads_deinit (c);
   if (err != 0)
      FAIL ("threads deinit");

   context_deinit (c);

   return EXIT_SUCCESS;
}
