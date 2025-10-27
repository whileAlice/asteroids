#include <raylib.h>

#include "input.h"
#include "context.h"

extern Context g_ctx;

void
update_input()
{
  g_ctx.input.show_osd          = IsKeyPressed(KEY_GRAVE) && !IsKeyDown(KEY_RIGHT_SHIFT);
  g_ctx.input.show_log      = IsKeyPressed(KEY_GRAVE) && IsKeyDown(KEY_RIGHT_SHIFT);
  g_ctx.input.log_page_up   = IsKeyPressed(KEY_PAGE_UP);
  g_ctx.input.log_page_down = IsKeyPressed(KEY_PAGE_DOWN);

  g_ctx.input.accelerate = IsKeyDown(KEY_W);
  g_ctx.input.decelerate = IsKeyDown(KEY_S);
  g_ctx.input.turn_left  = IsKeyDown(KEY_A);
  g_ctx.input.turn_right = IsKeyDown(KEY_D);
}
