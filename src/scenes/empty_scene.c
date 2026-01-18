#include "empty_scene.h"

#include "../context.h"
#include "../draw_utils.h"

#include <raylib.h>

bool
empty_scene_init (Context* c)
{
   return true;
}

bool
empty_scene_deinit (Context* c)
{
   return true;
}

void
empty_scene_update (Context* c, float dt)
{
}

void
empty_scene_draw (Context* c)
{
   clear_buffer (rgb_from_rgba (BLUE));
}
