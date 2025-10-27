#include <raylib.h>

#include "context.h"
#include "window_utils.h"
#include "config.h"
#include "math_utils.h"

extern Context g_ctx;

int
get_integer_scale()
{
  const int scale_x = GetRenderWidth()  / PIXEL_BUFFER_WIDTH;
  const int scale_y = GetRenderHeight() / PIXEL_BUFFER_HEIGHT;

  return MIN(scale_x, scale_y);
}

int
get_texture_origin_x()
{
   return GetRenderWidth() % PIXEL_BUFFER_WIDTH / 2;
}

int
get_texture_origin_y()
{
  return GetRenderHeight() % PIXEL_BUFFER_HEIGHT / 2;
}

int
get_mouse_x()
{
  const int mouse_x = GetMouseX() - g_ctx.buffer_texture_origin_x;

  return CLAMP(mouse_x / g_ctx.integer_scale, 0, PIXEL_BUFFER_WIDTH - 1);
}

int
get_mouse_y()
{
  const int mouse_y = GetMouseY() - g_ctx.buffer_texture_origin_y;

  return CLAMP(mouse_y / g_ctx.integer_scale, 0, PIXEL_BUFFER_HEIGHT - 1);
}
