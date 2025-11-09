#include <raylib.h>

#include "context.h"
#include "game_loop.h"
#include "window_utils.h"
#include "window_loop.h"

void
window_loop(Context* c) {
  c->buffer.integer_scale    = get_integer_scale();
  c->buffer.texture_origin_x = get_texture_origin_x();
  c->buffer.texture_origin_y = get_texture_origin_y();

  while (!c->state.should_exit_app && !WindowShouldClose()) {
    if (IsWindowResized()) {
      c->buffer.integer_scale    = get_integer_scale();
      c->buffer.texture_origin_x = get_texture_origin_x();
      c->buffer.texture_origin_y = get_texture_origin_y();
    }

    float dt = GetFrameTime();
    game_update(c, dt);
    game_draw(c, &c->buffer.image);

    UpdateTexture(c->buffer.texture, c->buffer.image.data);

    BeginDrawing();

      ClearBackground(BLACK);
      DrawTextureEx(c->buffer.texture,
                    (Vector2){
                      .x = (float)c->buffer.texture_origin_x,
                      .y = (float)c->buffer.texture_origin_y
                    },
                    .0,
                    (float)c->buffer.integer_scale,
                    WHITE);

    EndDrawing();
  }
}
