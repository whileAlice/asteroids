#pragma once

#include <stddef.h>
#include <raylib.h>

#include "font.h"

typedef struct input_context{
  bool show_osd;
  bool show_log;
  bool log_page_up;
  bool log_page_down;
  bool switch_fullscreen;
} InputContext;

typedef struct state_context{
  bool   show_osd;
  bool   show_log;
  bool   should_exit_app;
  size_t current_log_page;
} StateContext;

typedef struct font_context{
  FixedFont fixed_font;
  FixedFont inverted_fixed_font;
} FontContext;

typedef struct buffer_context{
  Image     image;
  Texture2D texture;
  int       texture_origin_x;
  int       texture_origin_y;
  int       integer_scale;
} BufferContext;

typedef struct context{
  FontContext   font;
  BufferContext buffer;
  InputContext  input;
  StateContext  state;
} Context;
