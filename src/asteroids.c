#include <raylib.h>

#include "init.h"
#include "window_loop.h"

int
main(void)
{
  init();
  window_loop();
  deinit();
}
