#include "window_loop.h"

#include "context.h"
#include "game_loop.h"
#include "window_utils.h"

#include <raylib.h>

void
window_loop (Context* c)
{
   c->pixel_buffer->integer_scale    = get_integer_scale ();
   c->pixel_buffer->texture_origin_x = get_texture_origin_x ();
   c->pixel_buffer->texture_origin_y = get_texture_origin_y ();

   while (!c->state->should_exit_app && !WindowShouldClose ())
   {
      if (IsWindowResized ())
      {
         c->pixel_buffer->integer_scale    = get_integer_scale ();
         c->pixel_buffer->texture_origin_x = get_texture_origin_x ();
         c->pixel_buffer->texture_origin_y = get_texture_origin_y ();
      }

      float dt = GetFrameTime ();
      game_update (c, dt);
      game_draw (c, &c->pixel_buffer->image);

      UpdateTexture (c->pixel_buffer->texture, c->pixel_buffer->image.data);

      BeginDrawing ();

      ClearBackground (BLACK);
      DrawTextureEx (c->pixel_buffer->texture,
                     (Vector2){ .x = (float)c->pixel_buffer->texture_origin_x,
                                .y = (float)c->pixel_buffer->texture_origin_y },
                     .0, (float)c->pixel_buffer->integer_scale, WHITE);

      EndDrawing ();
   }
}
