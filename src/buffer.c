#include "buffer.h"

#include "config.h"
#include "draw_utils.h"
#include "error.h"

#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#define PIXEL_COUNT PIXEL_BUFFER_WIDTH* PIXEL_BUFFER_HEIGHT

Image*
buffer_image_create (void)
{
   Image* image = malloc (sizeof (Image));
   if (image == NULL)
      ERRNO_RETURN (NULL, "image malloc");

   Color3* pixels = (Color3*)malloc (PIXEL_COUNT * sizeof (Color3));
   if (pixels == NULL)
      ERRNO_RETURN (NULL, "pixels malloc");

   memset (pixels, 0, sizeof (Color3));

   *image = (Image){
      .data    = pixels,
      .width   = PIXEL_BUFFER_WIDTH,
      .height  = PIXEL_BUFFER_HEIGHT,
      .mipmaps = 1,
      .format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
   };

   return image;
}

void
buffer_image_free (Image* image)
{
   free (image->data);
   free (image);
}
