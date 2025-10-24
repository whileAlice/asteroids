#include <raylib.h>

#include "game_loop.h"
#include "draw_utils.h"
#include "pam.h"
#include "window_utils.h"

int x;
int y;
Image pika;

// TODO: this should be init while the other one should be window_init
void
game_init()
{
  pika = image_from_pam("assets/pikachu_smol.pam");
}

void
update(float dt)
{
  x = get_mouse_x();
  y = get_mouse_y();
}

void
draw(Image *im)
{
  clear_image(im, c3_from_c4(GRAY));
  draw_triangle(im,
                (Vector2){ .x = 50.0f,  .y = 100.0f },
                (Vector2){ .x = 100.0f, .y = 40.0f },
                (Vector2){ .x = 200.0f, .y = 180.0f },
                c3_from_c4(RED), c3_from_c4(GREEN));
  draw_rectangle_fi(im, 20, 20, 50, 50, c3_from_c4(BLUE));
  draw_image(im, &pika, x, y);
}

void
game_deinit()
{
  UnloadImage(pika);
}
