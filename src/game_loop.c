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
  draw_triangle(im,
                (Vector2){ .x = 50.0f,    .y = 100.0f },
                (Vector2){ .x = (float)x, .y = (float)y },
                (Vector2){ .x = 200,      .y = 180 },
                RED, GREEN);
}
