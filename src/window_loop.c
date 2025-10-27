#include <raylib.h>

#include "game_loop.h"
#include "window_loop.h"
#include "window_utils.h"

extern Image     g_image;
extern Texture2D g_texture;

int g_integer_scale;
int g_texture_origin_x;
int g_texture_origin_y;

void
window_loop() {
  g_integer_scale    = get_integer_scale();
  g_texture_origin_x = get_texture_origin_x();
  g_texture_origin_y = get_texture_origin_y();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_F)) {
      ToggleBorderlessWindowed();
    }


    if (IsWindowResized()) {
      g_integer_scale    = get_integer_scale();
      g_texture_origin_x = get_texture_origin_x();
      g_texture_origin_y = get_texture_origin_y();
    }

    float dt = GetFrameTime();
    game_update(dt);
    game_draw(&g_image);

    UpdateTexture(g_texture, g_image.data);

    BeginDrawing();

      ClearBackground(BLACK);
      DrawTextureEx(g_texture,
                    (Vector2){
                      .x = (float)g_texture_origin_x,
                      .y = (float)g_texture_origin_y
                    },
                    .0,
                    (float)g_integer_scale,
                    WHITE);

    EndDrawing();
  }
}
