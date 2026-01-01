#include "buffer.h"
#include "config.h"
#include "context.h"
#include "game_loop.h"
#include "window_loop.h"

#include <raylib.h>

int
main (void)
{
   InitWindow (INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, APP_TITLE);

   SetExitKey (EXIT_KEY);
   SetTargetFPS (TARGET_FPS);
   HideCursor ();

   Context c = { 0 };

   c.buffer.image   = generate_buffer_image ();
   c.buffer.texture = LoadTextureFromImage (c.buffer.image);

   game_init (&c);
   window_loop (&c);
   game_deinit (&c);

   UnloadTexture (c.buffer.texture);
   UnloadImage (c.buffer.image);

   CloseWindow ();
}
