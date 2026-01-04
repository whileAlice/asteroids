#include "update_state.h"

#include "context.h"

#include <raylib.h>

void
update_state (Context* c)
{
   if (c->input->show_log)
   {
      c->state->should_show_log = !c->state->should_show_log;
   }
   if (c->input->show_osd)
   {
      c->state->should_show_osd = !c->state->should_show_osd;
   }
   if (c->input->switch_fullscreen)
   {
      ToggleBorderlessWindowed ();
   }
}
