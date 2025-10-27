#pragma once

typedef struct Input{
  bool accelerate;
  bool decelerate;
  bool turn_left;
  bool turn_right;
  bool show_console;
  bool show_osd;
} Input;

void update_input();
