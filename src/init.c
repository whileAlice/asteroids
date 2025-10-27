#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "context.h"
#include "config.h"
#include "draw_utils.h"
#include "game_loop.h"
#include "init.h"

#define PIXEL_COUNT PIXEL_BUFFER_WIDTH * PIXEL_BUFFER_HEIGHT

extern Context g_ctx;

void
init()
{
  Color3* pixels = (Color3*)malloc(PIXEL_COUNT * sizeof(Color3));
  if (pixels == NULL) {
    printf("ERROR: couldn't allocate buffer memory");
    exit(1);
  }

  for (size_t i = 0; i < PIXEL_COUNT; ++i) {
    pixels[i] = (Color3){
      .r = 18,
      .g = 18,
      .b = 12,
    };
  };

  g_ctx.buffer_image = (Image){
    .data    = pixels,
    .width   = PIXEL_BUFFER_WIDTH,
    .height  = PIXEL_BUFFER_HEIGHT,
    .mipmaps = 1,
    .format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
  };

  InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, APP_TITLE);
  SetExitKey(EXIT_KEY);
  SetTargetFPS(60);

  g_ctx.buffer_texture = LoadTextureFromImage(g_ctx.buffer_image);

  game_init();
}

void
deinit()
{
  game_deinit();

  UnloadTexture(g_ctx.buffer_texture);
  UnloadImage(g_ctx.buffer_image);

  CloseWindow();
}
