#include "main_menu_bg_scene.h"

#include "../context.h"
#include "../draw_utils.h"

#include <raylib.h>

bool
main_menu_bg_scene_init (Context* c)
{
   return true;
}

bool
main_menu_bg_scene_deinit ()
{
   return true;
}

void
main_menu_bg_scene_update (Context* c, float dt)
{
}

void
main_menu_bg_scene_draw (Context* c)
{
   clear_buffer (rgb_from_rgba (BLUE));
}
