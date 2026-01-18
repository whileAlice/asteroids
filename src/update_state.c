#include "update_state.h"

#include "context.h"
#include "threads.h"

#include <raylib.h>

void
update_state (Context* c)
{
   if (c->input->toggle_log)
   {
      c->state->should_show_log = !c->state->should_show_log;
      c->input->toggle_log = false;
   }

   if (c->input->toggle_osd)
   {
      c->state->should_show_osd = !c->state->should_show_osd;
      c->input->toggle_osd = false;
   }

   if (c->input->switch_fullscreen)
      ToggleBorderlessWindowed ();
}
