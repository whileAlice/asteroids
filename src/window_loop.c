#include <raylib.h>

#include "context.h"
#include "game_loop.h"
#include "window_utils.h"
#include "window_loop.h"

void
window_loop(Context* c) {
  c->integer_scale           = get_integer_scale();
  c->buffer_texture_origin_x = get_texture_origin_x();
  c->buffer_texture_origin_y = get_texture_origin_y();

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      c->integer_scale           = get_integer_scale();
      c->buffer_texture_origin_x = get_texture_origin_x();
      c->buffer_texture_origin_y = get_texture_origin_y();
    }

    float dt = GetFrameTime();
    game_update(c, dt);
    game_draw(c, &c->buffer_image);

    UpdateTexture(c->buffer_texture, c->buffer_image.data);

    BeginDrawing();

      ClearBackground(BLACK);
      DrawTextureEx(c->buffer_texture,
                    (Vector2){
                      .x = (float)c->buffer_texture_origin_x,
                      .y = (float)c->buffer_texture_origin_y
                    },
                    .0,
                    (float)c->integer_scale,
                    WHITE);

    EndDrawing();
  }
}
