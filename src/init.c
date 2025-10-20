#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "config.h"
#include "draw_utils.h"
#include "init.h"

#define PIXEL_COUNT BUFFER_WIDTH * BUFFER_HEIGHT

Image image;
Texture2D texture;

void
init()
{
  Color3 *pixels = (Color3 *)malloc(PIXEL_COUNT * sizeof(Color3));
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

  image = (Image){
    .data = pixels,
    .width = BUFFER_WIDTH,
    .height = BUFFER_HEIGHT,
    .mipmaps = 1,
    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
  };

  InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, APP_TITLE);
  SetExitKey(EXIT_KEY);
  SetTargetFPS(60);

  texture = LoadTextureFromImage(image);
}

void
deinit()
{
  UnloadTexture(texture);
  UnloadImage(image);

  CloseWindow();
}
