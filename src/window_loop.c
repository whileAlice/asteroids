#include <raylib.h>

#include "game_loop.h"
#include "window_loop.h"
#include "window_utils.h"

extern Image image;
extern Texture2D texture;
int integer_scale;
int texture_origin_x;
int texture_origin_y;

void
window_loop() {
  integer_scale = get_integer_scale();
  texture_origin_x = get_texture_origin_x();
  texture_origin_y = get_texture_origin_y();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_F)) {
      ToggleBorderlessWindowed();
    }


    if (IsWindowResized()) {
      integer_scale = get_integer_scale();
      texture_origin_x = get_texture_origin_x();
      texture_origin_y = get_texture_origin_y();
    }

    float dt = GetFrameTime();
    update(dt);
    draw(&image);

    UpdateTexture(texture, image.data);

    BeginDrawing();

      ClearBackground(BLACK);
      DrawTextureEx(texture,
                    (Vector2){
                      .x = (float)texture_origin_x,
                      .y = (float)texture_origin_y
                    },
                    .0,
                    (float)integer_scale,
                    WHITE);

    EndDrawing();
  }
}
