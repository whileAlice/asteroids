#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "config.h"
#include "draw_utils.h"
#include "buffer.h"

#define PIXEL_COUNT PIXEL_BUFFER_WIDTH * PIXEL_BUFFER_HEIGHT

Image
generate_buffer_image()
{
  Color3* pixels = (Color3*)malloc(PIXEL_COUNT * sizeof(Color3));
  if (pixels == NULL) {
    printf("ERROR: couldn't allocate buffer memory");
    exit(1);
  }

  memset(pixels, 0, sizeof(Color3));

  return (Image){
    .data    = pixels,
    .width   = PIXEL_BUFFER_WIDTH,
    .height  = PIXEL_BUFFER_HEIGHT,
    .mipmaps = 1,
    .format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
  };
}
