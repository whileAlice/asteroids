#include <stddef.h>

#include "font.h"

FixedFont
load_fixed_font(Image* glyph_sheet, Padding glyph_padding, int glyph_width,
                int glyph_height, int tail_height, size_t glyph_count)
{
  return (FixedFont){
    .glyph_sheet   = glyph_sheet,
    .glyph_padding = glyph_padding,
    .glyph_width   = glyph_width,
    .glyph_height  = glyph_height,
    .tail_height   = tail_height,
    .glyph_count   = glyph_count,
  };
}
