#include <raylib.h>

#include "context.h"
#include "window_utils.h"
#include "config.h"
#include "math_utils.h"

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
get_mouse_x(Context* c)
{
  const int mouse_x = GetMouseX() - c->buffer_texture_origin_x;

  return CLAMP(mouse_x / c->integer_scale, 0, PIXEL_BUFFER_WIDTH - 1);
}

int
get_mouse_y(Context* c)
{
  const int mouse_y = GetMouseY() - c->buffer_texture_origin_y;

  return CLAMP(mouse_y / c->integer_scale, 0, PIXEL_BUFFER_HEIGHT - 1);
}
