#pragma once

#include <stddef.h>
#include <stdio.h>

#include "raylib.h"

typedef struct Margin{
  int top;
  int bottom;
  int left;
  int right;
} Margin;

typedef struct FixedFont{
  Image* glyph_sheet;
  Margin glyph_padding;
  int    glyph_width;
  int    glyph_height;
  size_t glyph_count;
} FixedFont;

void load_fixed_font(const char* font_def);
void unload_fixed_font();
void read_all_font_tokens(FILE* fp, char* buf, const char* filename);
