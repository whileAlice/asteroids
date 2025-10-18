#include <raylib.h>
#include <stdio.h>

#include "game_loop.h"
#include "draw_utils.h"
#include "window_utils.h"

// init
size_t x;
size_t y;

void
update(float dt)
{
  x = get_mouse_x();
  y = get_mouse_y();
}

void
draw(Image *image)
{
  clear_image(image, GRAY);
  draw_line(image, 100, 100, x, y, RED);
}
