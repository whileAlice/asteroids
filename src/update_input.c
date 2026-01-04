#include "update_input.h"

#include "context.h"

#include <raylib.h>

void
update_input (Context* c)
{
   c->input->show_osd = IsKeyPressed (KEY_GRAVE) && !IsKeyDown (KEY_RIGHT_SHIFT);
   c->input->show_log = IsKeyPressed (KEY_GRAVE) && IsKeyDown (KEY_RIGHT_SHIFT);
   c->input->log_page_up       = IsKeyPressed (KEY_PAGE_UP);
   c->input->log_page_down     = IsKeyPressed (KEY_PAGE_DOWN);
   c->input->switch_fullscreen = IsKeyPressed (KEY_F);
}
