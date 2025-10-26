#include <assert.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "config.h"
#include "draw_utils.h"
#include "font.h"

#define CHAR_SPACING   1
#define PADDING_TOP    3
#define PADDING_BOTTOM 1
#define PADDING_LEFT   3
#define PADDING_RIGHT  1
#define PAGE_COUNT     3

#define PADDING_TB (PADDING_TOP  + PADDING_BOTTOM)
#define PADDING_LR (PADDING_LEFT + PADDING_RIGHT)

static size_t     page_size;
static size_t     console_buffer_size;
static size_t     console_buffer_line_count;
static size_t     col_count;
static size_t     row_count;
static char*      osd_buffer;
static char*      console_buffer;
static FixedFont* g_font;
static size_t     current_page = PAGE_COUNT;

void
init_log_buffers(FixedFont* font)
{
  assert(PIXEL_BUFFER_WIDTH  - PADDING_LR >= (size_t)font->glyph_width);
  assert(PIXEL_BUFFER_HEIGHT - PADDING_TB >= (size_t)font->glyph_height);

  col_count =
    (PIXEL_BUFFER_WIDTH  - PADDING_LR - font->glyph_width) /
    (font->glyph_width  + CHAR_SPACING) + 2;
  row_count =
    (PIXEL_BUFFER_HEIGHT - PADDING_TB - font->glyph_height) /
    (font->glyph_height + CHAR_SPACING) + 1;

  page_size                 = col_count * row_count;
  console_buffer_size       = page_size * PAGE_COUNT;
  console_buffer_line_count = row_count * PAGE_COUNT;

  console_buffer = malloc(console_buffer_size * sizeof(char));
  osd_buffer     = malloc(page_size           * sizeof(char));

  memset(console_buffer, ' ', console_buffer_size * sizeof(char));
  memset(osd_buffer,     ' ', page_size           * sizeof(char));

  for (size_t i = 1; i <= console_buffer_line_count; ++i) {
    console_buffer[i * col_count - 1] = '\0';
  }

  g_font = font;
}

void
add_line_to_console_log(const char* line)
{
  for (size_t i = 0; i < console_buffer_line_count - 1; ++i) {
    size_t char_count = col_count;
    char* dst = &console_buffer[i * col_count];
    char* src = &console_buffer[(i + 1) * col_count];

    while (char_count--) *dst++ = *src++;
  }

  strcpy(&console_buffer[(console_buffer_line_count - 1) * col_count], line);
}

void
show_console_log(Image* buf)
{
  for (size_t i = 0; i < row_count; ++i) {
    const char* current_line =
      &console_buffer[(current_page - 1) * page_size + i * col_count];
    const int origin_y =
      PADDING_TOP + (int)i * (g_font->glyph_height + CHAR_SPACING);

    draw_text(buf, g_font, PADDING_LEFT, origin_y, current_line, CHAR_SPACING);
  }
}

void
deinit_log_buffers()
{
  free(console_buffer);
  free(osd_buffer);
}
