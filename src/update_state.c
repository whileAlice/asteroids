#include <raylib.h>

#include "context.h"
#include "update_state.h"

void
update_state(Context* c)
{
  if (c->input.show_log) {
    c->state.show_log = !c->state.show_log;
  }
  if (c->input.show_osd) {
    c->state.show_osd = !c->state.show_osd;
  }
  if (c->input.switch_fullscreen) {
    ToggleBorderlessWindowed();
  }
}
