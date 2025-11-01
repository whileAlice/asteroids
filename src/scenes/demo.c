#include <raylib.h>

#include "demo.h"
#include "../context.h"
#include "../draw_utils.h"

void
demo_init(Context* c){}

void
demo_update(Context* c, float dt)
{

}

void
demo_draw(Context* c, Image* buf)
{
  clear_rgb_image(buf, rgb_from_rgba(PINK));
  draw_triangle(buf,
                (Vector2){ .x = 50.0f,  .y = 100.0f },
                (Vector2){ .x = 100.0f, .y = 40.0f },
                (Vector2){ .x = 200.0f, .y = 180.0f },
                MAROON, DARKPURPLE);
  draw_rectangle_fi(buf, 20, 20, 50, 50, BLUE);
}

void
demo_deinit()
{

}
