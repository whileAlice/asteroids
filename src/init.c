#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "config.h"
#include "pam.h"
#include "init.h"

#define PIXEL_COUNT BUFFER_WIDTH * BUFFER_HEIGHT

Image image;
Image img;
Texture2D texture;

void
init()
{
  Color *pixels = (Color *)malloc(PIXEL_COUNT * sizeof(Color));
  if (pixels == NULL) {
    printf("ERROR: couldn't allocate buffer memory");
    exit(1);
  }

  for (size_t i = 0; i < PIXEL_COUNT; ++i) {
    pixels[i] = BLUE;
  };

  img = image_from_pam("assets/pikachu_smol.pam");

  image = (Image){
    .data = img.data,
    .width = BUFFER_WIDTH,
    .height = BUFFER_HEIGHT,
    .mipmaps = 1,
    // TODO: can this possibly be R8G8B8 without much hassle?
    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, APP_TITLE);
  SetExitKey(EXIT_KEY);
  SetTargetFPS(60);

  texture = LoadTextureFromImage(img);
}

void
deinit()
{
  UnloadTexture(texture);
  UnloadImage(image);
  UnloadImage(img);

  CloseWindow();
}
