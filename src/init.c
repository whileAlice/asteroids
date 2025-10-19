#include <stdlib.h>
#include <raylib.h>

#include "config.h"
#include "init.h"

#define PIXEL_COUNT BUFFER_WIDTH * BUFFER_HEIGHT

Image image;
Texture2D texture;

void
init()
{
  Color *pixels = (Color *)malloc(PIXEL_COUNT * sizeof(Color));

  for (size_t i = 0; i < PIXEL_COUNT; ++i) {
    pixels[i] = RED;
  };

  image = (Image){
    .data = pixels,
    .width = BUFFER_WIDTH,
    .height = BUFFER_HEIGHT,
    .mipmaps = 1,
    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
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
