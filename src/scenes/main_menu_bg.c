#include <raylib.h>

#include "main_menu_bg.h"
#include "../context.h"
#include "../draw_utils.h"

void
main_menu_bg_scene_init(Context* c)
{
}

void
main_menu_bg_scene_deinit()
{
}

void
main_menu_bg_scene_update(Context* c, float dt)
{
}

void
main_menu_bg_scene_draw(Context* c, Image* buf)
{
  clear_rgb_image(buf, rgb_from_rgba(BLUE));
}
