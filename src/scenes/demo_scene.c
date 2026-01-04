#include "demo_scene.h"

#include "../context.h"
#include "../draw_utils.h"
#include "../pam.h"

#include <raylib.h>
#include <raymath.h>

static Image   s_hornet_image;
static Vector2 s_pos, s_vel;

void
demo_scene_init (Context* c)
{
   s_pos = (Vector2){ .x = 0.f, .y = 0.f };
   s_vel = (Vector2){ .x = 20.f, .y = 20.f };

   s_hornet_image = image_from_pam ("assets/semihornet.pam");
}

void
demo_scene_deinit ()
{
   UnloadImage (s_hornet_image);
}

void
demo_scene_update (Context* c, float dt)
{
   s_pos = Vector2Add (s_pos, Vector2Scale (s_vel, dt));

   float right_limit = (float)(c->pixel_buffer->image.width - s_hornet_image.width);
   if (s_pos.x >= right_limit)
   {
      s_pos.x = right_limit - 1;
      s_vel.x = -s_vel.x;
   }
   float bottom_limit = (float)(c->pixel_buffer->image.height - s_hornet_image.height);
   if (s_pos.y >= bottom_limit)
   {
      s_pos.y = bottom_limit - 1;
      s_vel.y = -s_vel.y;
   }
   if (s_pos.x < 0.f)
   {
      s_pos.x = 0.f;
      s_vel.x = -s_vel.x;
   }
   if (s_pos.y < 0.f)
   {
      s_pos.y = 0.f;
      s_vel.y = -s_vel.y;
   }
}

void
demo_scene_draw (Context* c, Image* buf)
{
   clear_rgb_image (buf, rgb_from_rgba (PINK));

   draw_rectangle (buf, (Vector2){ .x = 20.f, .y = 20.f },
                   (Vector2){ .x = 100.f, .y = 100.f }, BLUE, DARKBLUE);
   draw_triangle (buf, (Vector2){ .x = 50.0f, .y = 100.0f },
                  (Vector2){ .x = 100.0f, .y = 40.0f },
                  (Vector2){ .x = 200.0f, .y = 180.0f }, MAROON, DARKPURPLE);
   draw_line (buf, (Vector2){ .x = 310.f, .y = 20.f },
              (Vector2){ .x = 200.f, .y = 180.f }, RED);
   draw_rgba_image (buf, &s_hornet_image, s_pos);
}
