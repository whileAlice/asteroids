#pragma once

#include <raylib.h>

#include "font.h"

typedef struct Context{
  FixedFont fixed_font;
  Image     buffer_image;
  Texture2D buffer_texture;
  int       buffer_texture_origin_x;
  int       buffer_texture_origin_y;
  int       integer_scale;
} Context;
