#include <raylib.h>

#include "input.h"
#include "context.h"

extern Context g_ctx;

void
update_input()
{
  g_ctx.input.system.show_osd     = IsKeyPressed(KEY_GRAVE) && !IsKeyDown(KEY_RIGHT_SHIFT);
  g_ctx.input.system.show_console = IsKeyPressed(KEY_GRAVE) && IsKeyDown(KEY_RIGHT_SHIFT);

  g_ctx.input.player.accelerate   = IsKeyPressed(KEY_W);
  g_ctx.input.player.decelerate   = IsKeyPressed(KEY_S);
  g_ctx.input.player.turn_left    = IsKeyPressed(KEY_A);
  g_ctx.input.player.turn_right   = IsKeyPressed(KEY_D);
}
