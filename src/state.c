#include "context.h"
#include "state.h"

extern Context g_ctx;

void
update_state()
{
  if (g_ctx.input.show_log) {
    g_ctx.state.show_log = !g_ctx.state.show_log;
  }
  if (g_ctx.input.show_osd) {
    g_ctx.state.show_osd = !g_ctx.state.show_osd;
  }
}
