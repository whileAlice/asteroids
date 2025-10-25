#pragma once

#include <stddef.h>

#include "raylib.h"

typedef struct Padding{
  int top;
  int bottom;
  int left;
  int right;
} Padding;

typedef struct FixedFont{
  Image*  glyph_sheet;
  Padding glyph_padding;
  int     glyph_width;
  int     glyph_height;
  int     tail_height;
  size_t  glyph_count;
} FixedFont;

FixedFont load_fixed_font(Image* glyph_sheet, Padding glyph_padding,
                          int glyph_width, int glyph_height,
                          int tail_height, size_t glyph_count);
