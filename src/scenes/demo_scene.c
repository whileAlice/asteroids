#include "demo_scene.h"

#include "../context.h"
#include "../draw_utils.h"
#include "../pam.h"

#include <raylib.h>
#include <raymath.h>

static Image*  s_hornet_image;
static Vector2 s_pos, s_vel;

bool
demo_scene_init (Context* c)
{
   s_pos = (Vector2){ .x = 0.f, .y = 0.f };
   s_vel = (Vector2){ .x = 20.f, .y = 20.f };

   s_hornet_image = image_from_pam ("assets/semihornet.pam");
   if (s_hornet_image == NULL)
      ERROR_RETURN (false, "image from pam");

   return true;
}

bool
demo_scene_deinit (Context* c)
{
   UnloadImage (*s_hornet_image);
   free (s_hornet_image);

   return true;
}

void
demo_scene_update (Context* c, float dt)
{
   s_pos = Vector2Add (s_pos, Vector2Scale (s_vel, dt));

   float right_limit =
      (float)(c->pixel_buffer->image->width - s_hornet_image->width);
   if (s_pos.x >= right_limit)
   {
      s_pos.x = right_limit - 1;
      s_vel.x = -s_vel.x;
   }
   float bottom_limit =
      (float)(c->pixel_buffer->image->height - s_hornet_image->height);
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
demo_scene_draw (Context* c)
{
   clear_buffer (rgb_from_rgba (PINK));

   draw_rectangle ((Vector2){ 20, 20 }, (Vector2){ 100, 100 }, BLUE, DARKBLUE);
   draw_triangle ((Vector2){ 50, 100 }, (Vector2){ 100, 40 },
                  (Vector2){ 200, 180 }, MAROON, DARKPURPLE);
   draw_line ((Vector2){ 310, 20 }, (Vector2){ 200, 180 }, RED);
   draw_rgba_image (s_pos, s_hornet_image);
}
