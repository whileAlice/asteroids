#include <raylib.h>

#include "game_loop.h"
#include "draw_utils.h"
#include "window_utils.h"

// init
int x;
int y;

void
update(float dt)
{
  x = get_mouse_x();
  y = get_mouse_y();
}

void
draw(Image *im)
{
  clear_image(im, GRAY);
  draw_triangle_f(im, x, y, 20, 150, 240, 140, RED);
}
