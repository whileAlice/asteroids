#include "update_state.h"

#include "context.h"
#include "event.h"
#include "threads.h"

#include <raylib.h>

// TODO: should this be merged with update_input?
void
update_state (Context* c)
{
   if (c->input->toggle_log)
   {
      c->state->should_show_log = !c->state->should_show_log;
      // TODO: improve this
      c->state->has_overlay = c->state->should_show_log;
   }

   if (c->input->toggle_osd)
      c->state->should_show_osd = !c->state->should_show_osd;

   if (c->input->toggle_main_menu)
      toggle_main_menu (c);

   if (c->input->switch_fullscreen)
      ToggleBorderlessWindowed ();
}
