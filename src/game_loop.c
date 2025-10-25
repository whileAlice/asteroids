#include <raylib.h>

#include "game_loop.h"
#include "draw_utils.h"
#include "pam.h"
#include "window_utils.h"

static int mouse_x;
static int mouse_y;
static Image pika;
static Image hornet;

void
game_init()
{
  pika = image_from_pam("assets/pikachu_smol.pam");
  hornet = image_from_pam("assets/semihornet.pam");
}

void
game_update(float dt)
{
  mouse_x = get_mouse_x();
  mouse_y = get_mouse_y();
}

void
game_draw(Image* buf)
{
  clear_image_rgb(buf, rgb_from_rgba(GRAY));
  draw_triangle(buf,
                (Vector2){ .x = 50.0f,  .y = 100.0f },
                (Vector2){ .x = 100.0f, .y = 40.0f },
                (Vector2){ .x = 200.0f, .y = 180.0f },
                RED, GREEN);
  draw_rectangle_fi(buf, 20, 20, 50, 50, BLUE);
  draw_image_a(buf, &hornet, mouse_x, mouse_y);
}

void
game_deinit()
{
  UnloadImage(hornet);
  UnloadImage(pika);
}
