#pragma once

#include "raylib.h"

#include <stddef.h>
#include <stdio.h>

typedef struct margins {
   int top;
   int bottom;
   int left;
   int right;
} Margins;

typedef struct fixed_font {
   Image*  glyph_sheet;
   Margins glyph_margins;
   int     glyph_spacing;
   int     glyph_width;
   int     glyph_height;
   size_t  glyph_count;
} FixedFont;

void load_fixed_fonts (FixedFont* fixed_font, FixedFont* fixed_font_inverted,
                       const char* font_def);
void unload_fixed_font_images ();
void read_all_font_tokens (FILE* fp, char* buf, const char* filename);
