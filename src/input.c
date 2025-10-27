#include <raylib.h>

#include "input.h"
#include "context.h"

extern Context g_ctx;

void
update_input()
{
  g_ctx.input.show_osd     = IsKeyPressed(KEY_GRAVE) && !IsKeyDown(KEY_RIGHT_SHIFT);
  g_ctx.input.show_console = IsKeyPressed(KEY_GRAVE) && IsKeyDown(KEY_RIGHT_SHIFT);

  g_ctx.input.accelerate   = IsKeyPressed(KEY_W);
  g_ctx.input.decelerate   = IsKeyPressed(KEY_S);
  g_ctx.input.turn_left    = IsKeyPressed(KEY_A);
  g_ctx.input.turn_right   = IsKeyPressed(KEY_D);
}
