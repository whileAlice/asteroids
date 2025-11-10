#include <raylib.h>

#include "config.h"
#include "context.h"
#include "buffer.h"
#include "game_loop.h"
#include "window_loop.h"

int
main(void)
{
  Context c = {0};

  c.buffer.image = generate_buffer_image();

  InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, APP_TITLE);

  c.buffer.texture = LoadTextureFromImage(c.buffer.image);

  SetExitKey(EXIT_KEY);
  SetTargetFPS(TARGET_FPS);
  HideCursor();

  game_init(&c);

  window_loop(&c);

  game_deinit(&c);

  UnloadTexture(c.buffer.texture);
  UnloadImage(c.buffer.image);

  CloseWindow();
}
