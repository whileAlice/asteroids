#include "window_utils.h"

#include "config.h"
#include "context.h"
#include "event.h"
#include "math_utils.h"

#include <raylib.h>

int
get_integer_scale (void)
{
   const int scale_x = GetScreenWidth () / PIXEL_BUFFER_WIDTH;
   const int scale_y = GetScreenHeight () / PIXEL_BUFFER_HEIGHT;

   return MIN (scale_x, scale_y);
}

int
get_texture_origin_x (void)
{
   return GetScreenWidth () % PIXEL_BUFFER_WIDTH / 2;
}

int
get_texture_origin_y (void)
{
   return GetScreenHeight () % PIXEL_BUFFER_HEIGHT / 2;
}

void
set_buffer_scale_and_texture_origin (Context* c)
{
   c->pixel_buffer->integer_scale    = get_integer_scale ();
   c->pixel_buffer->texture_origin_x = get_texture_origin_x ();
   c->pixel_buffer->texture_origin_y = get_texture_origin_y ();
}

int
get_mouse_x (Context* c)
{
   const int mouse_x = buffer_x_from_screen_x (c, GetMouseX ());

   return CLAMP (mouse_x, 0, PIXEL_BUFFER_WIDTH - 1);
}

int
get_mouse_y (Context* c)
{
   const int mouse_y = buffer_y_from_screen_y (c, GetMouseY ());

   return CLAMP (mouse_y, 0, PIXEL_BUFFER_HEIGHT - 1);
}

int
buffer_x_from_screen_x (Context* c, int screen_x)
{
   const int buf_x = screen_x - c->pixel_buffer->texture_origin_x;

   return buf_x / c->pixel_buffer->integer_scale;
}

int
buffer_y_from_screen_y (Context* c, int screen_y)
{
   const int buf_y = screen_y - c->pixel_buffer->texture_origin_y;

   return buf_y / c->pixel_buffer->integer_scale;
}

int
screen_x_from_buffer_x (Context* c, int buf_x)
{
   const int screen_x = buf_x * c->pixel_buffer->integer_scale;

   return screen_x + c->pixel_buffer->texture_origin_x;
}

int
screen_y_from_buffer_y (Context* c, int buf_y)
{
   const int screen_y = buf_y * c->pixel_buffer->integer_scale;

   return screen_y + c->pixel_buffer->texture_origin_y;
}

void
set_mouse_pos (Context* c, Vector2 pos)
{
   const int mouse_x = screen_x_from_buffer_x (c, (int)pos.x);
   const int mouse_y = screen_y_from_buffer_y (c, (int)pos.y);

   SetMousePosition (mouse_x, mouse_y);
}

bool
should_close_window (Context* c)
{
   if (!WindowShouldClose ())
      return false;

   app_quit_initiate (c);
   return true;
}
