#pragma once

#include <stddef.h>

typedef struct State{
  bool   show_osd;
  bool   show_log;
  size_t current_log_page;
} State;
