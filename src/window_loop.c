#include <raylib.h>

#include "context.h"
#include "game_loop.h"
#include "window_utils.h"
#include "window_loop.h"

extern Context g_ctx;

void
window_loop() {
  g_ctx.integer_scale           = get_integer_scale();
  g_ctx.buffer_texture_origin_x = get_texture_origin_x();
  g_ctx.buffer_texture_origin_y = get_texture_origin_y();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_F)) {
      ToggleBorderlessWindowed();
    }


    if (IsWindowResized()) {
      g_ctx.integer_scale           = get_integer_scale();
      g_ctx.buffer_texture_origin_x = get_texture_origin_x();
      g_ctx.buffer_texture_origin_y = get_texture_origin_y();
    }

    float dt = GetFrameTime();
    game_update(dt);
    game_draw(&g_ctx.buffer_image);

    UpdateTexture(g_ctx.buffer_texture, g_ctx.buffer_image.data);

    BeginDrawing();

      ClearBackground(BLACK);
      DrawTextureEx(g_ctx.buffer_texture,
                    (Vector2){
                      .x = (float)g_ctx.buffer_texture_origin_x,
                      .y = (float)g_ctx.buffer_texture_origin_y
                    },
                    .0,
                    (float)g_ctx.integer_scale,
                    WHITE);

    EndDrawing();
  }
}
