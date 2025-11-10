#pragma once

#include <stddef.h>
#include <raylib.h>

#include "font.h"

typedef struct context {
  struct {
    bool show_osd;
    bool show_log;
    bool log_page_up;
    bool log_page_down;
    bool switch_fullscreen;
  } input;
  struct {
    bool   show_osd;
    bool   show_log;
    bool   should_exit_app;
    size_t current_log_page;
  } state;
  struct {
    FixedFont fixed_font;
    FixedFont inverted_fixed_font;
  } font;
  struct {
    Image     image;
    Texture2D texture;
    int       texture_origin_x;
    int       texture_origin_y;
    int       integer_scale;
  } buffer;
} Context;
