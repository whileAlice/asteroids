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

static size_t    s_page_size;
static size_t    s_paged_buffer_size;
static size_t    s_paged_buffer_line_count;
static size_t    s_page_col_count;
static size_t    s_page_row_count;
static char*     s_osd_buffer;
static char*     s_console_buffer;
static size_t    s_current_page = PAGE_COUNT;

extern FixedFont g_fixed_font;

void
init_log_buffers()
{
  assert(PIXEL_BUFFER_WIDTH  - PADDING_LR >=
         (size_t)g_fixed_font.glyph_width);
  assert(PIXEL_BUFFER_HEIGHT - PADDING_TB >=
         (size_t)g_fixed_font.glyph_height);

  s_page_col_count =
    (PIXEL_BUFFER_WIDTH  - PADDING_LR - g_fixed_font.glyph_width) /
    (g_fixed_font.glyph_width  + CHAR_SPACING) + 2;
  s_page_row_count =
    (PIXEL_BUFFER_HEIGHT - PADDING_TB - g_fixed_font.glyph_height) /
    (g_fixed_font.glyph_height + CHAR_SPACING) + 1;

  s_page_size               = s_page_col_count * s_page_row_count;
  s_paged_buffer_size       = s_page_size * PAGE_COUNT;
  s_paged_buffer_line_count = s_page_row_count * PAGE_COUNT;

  s_console_buffer = malloc(s_paged_buffer_size * sizeof(char));
  s_osd_buffer     = malloc(s_page_size         * sizeof(char));

  memset(s_console_buffer, ' ', s_paged_buffer_size * sizeof(char));
  memset(s_osd_buffer,     ' ', s_page_size         * sizeof(char));

  for (size_t i = 1; i <= s_paged_buffer_line_count; ++i) {
    s_console_buffer[i * s_page_col_count - 1] = '\0';
    s_osd_buffer    [i * s_page_col_count - 1] = '\0';
  }
}

void
add_line_to_console_log(const char* line)
{
  for (size_t i = 0; i < s_paged_buffer_line_count - 1; ++i) {
    size_t char_count = s_page_col_count;
    char* dst = &s_console_buffer[i * s_page_col_count];
    char* src = &s_console_buffer[(i + 1) * s_page_col_count];

    while (char_count--) *dst++ = *src++;
  }

  const size_t last_line_index =
    (s_paged_buffer_line_count - 1) * s_page_col_count;

  strcpy(&s_console_buffer[last_line_index], line);
}

void
draw_console_log(Image* buf)
{
  for (size_t i = 0; i < s_page_row_count; ++i) {
    const char* current_line =
      &s_console_buffer[(s_current_page - 1) *
      s_page_size + i * s_page_col_count];
    const int origin_y =
      PADDING_TOP + (int)i * (g_fixed_font.glyph_height + CHAR_SPACING);

    draw_text(buf, &g_fixed_font, PADDING_LEFT, origin_y,
              current_line, CHAR_SPACING);
  }
}

void
print_to_osd(const char* text, size_t row, size_t col)
{
  strcpy(&s_osd_buffer[row * s_page_col_count + col], text);
}

void
draw_osd(Image* buf)
{
  for (size_t i = 0; i < s_page_row_count; ++i) {
    const char* current_line =
      &s_osd_buffer[i * s_page_col_count];
    const int origin_y =
      PADDING_TOP + (int)i * (g_fixed_font.glyph_height + CHAR_SPACING);

    draw_text(buf, &g_fixed_font, PADDING_LEFT, origin_y,
              current_line, CHAR_SPACING);
  }
}

void
deinit_log_buffers()
{
  free(s_console_buffer);
  free(s_osd_buffer);
}
