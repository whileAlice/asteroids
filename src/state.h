#pragma once

#include <stddef.h>

typedef struct State{
  bool   show_log;
  size_t current_log_page;
  bool   show_osd;
} State;

void update_state();
