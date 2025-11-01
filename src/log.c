#include <assert.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "log.h"
#include "math_utils.h"
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

static size_t s_page_size;
static size_t s_paged_buffer_size;
static size_t s_paged_buffer_line_count;
static size_t s_page_col_count;
static size_t s_page_row_count;
static char*  s_osd_buffer;
static char*  s_log_buffer;

void
init_log_buffers(Context* c)
{
  assert(c->fixed_font.glyph_count > 0);
  assert(PIXEL_BUFFER_WIDTH  - PADDING_LR >=
         (size_t)c->fixed_font.glyph_width);
  assert(PIXEL_BUFFER_HEIGHT - PADDING_TB >=
         (size_t)c->fixed_font.glyph_height);

  s_page_col_count =
    (PIXEL_BUFFER_WIDTH  - PADDING_LR - c->fixed_font.glyph_width) /
    (c->fixed_font.glyph_width  + CHAR_SPACING) + 2;
  s_page_row_count =
    (PIXEL_BUFFER_HEIGHT - PADDING_TB - c->fixed_font.glyph_height) /
    (c->fixed_font.glyph_height + CHAR_SPACING) + 1;

  s_page_size               = s_page_col_count * s_page_row_count;
  s_paged_buffer_size       = s_page_size * PAGE_COUNT;
  s_paged_buffer_line_count = s_page_row_count * PAGE_COUNT;

  s_log_buffer = malloc(s_paged_buffer_size * sizeof(char));
  s_osd_buffer = malloc(s_page_size         * sizeof(char));

  memset(s_log_buffer, ' ', s_paged_buffer_size * sizeof(char));
  memset(s_osd_buffer, ' ', s_page_size         * sizeof(char));

  for (size_t i = 1; i <= s_paged_buffer_line_count; ++i) {
    s_log_buffer[i * s_page_col_count - 1] = '\0';
    s_osd_buffer[i * s_page_col_count - 1] = '\0';
  }

  c->state.current_log_page = PAGE_COUNT;
}

void
osd_print(size_t row, size_t col, const char* text)
{
  strcpy(&s_osd_buffer[row * s_page_col_count + col], text);
}

void
osd_printf(size_t row, size_t col, const char* fmt, ...)
{
  char buf[1024];

  va_list args;
  va_start(args, fmt);

  vsnprintf(buf, 1024 * sizeof(char), fmt, args);
  osd_print(row, col, buf);

  va_end(args);
}

void
log_print(const char* line)
{
  for (size_t i = 0; i < s_paged_buffer_line_count - 1; ++i) {
    size_t char_count = s_page_col_count;
    char* dst = &s_log_buffer[i * s_page_col_count];
    char* src = &s_log_buffer[(i + 1) * s_page_col_count];

    while (char_count--) *dst++ = *src++;
  }

  const size_t last_line_index =
    (s_paged_buffer_line_count - 1) * s_page_col_count;

  strcpy(&s_log_buffer[last_line_index], line);
}

void
log_printf(const char* fmt, ...)
{
  char buf[1024];

  va_list args;
  va_start(args, fmt);

  vsnprintf(buf, 1024 * sizeof(char), fmt, args);
  log_print(buf);

  va_end(args);
}

void
draw_log(Context* c, Image* buf, bool is_inverted)
{
  FixedFont* font = is_inverted ?
                    &c->fixed_font_inverted :
                    &c->fixed_font;

  for (size_t i = 0; i < s_page_row_count; ++i) {
    const char* current_line =
      &s_log_buffer[(c->state.current_log_page - 1) *
      s_page_size + i * s_page_col_count];
    const int origin_y =
      PADDING_TOP + (int)i * (font->glyph_height + CHAR_SPACING);

    draw_text(buf, font, PADDING_LEFT, origin_y,
              current_line, CHAR_SPACING);
  }
}

void
draw_osd(Context* c, Image* buf, bool is_inverted)
{
  FixedFont* font = is_inverted ?
                    &c->fixed_font_inverted :
                    &c->fixed_font;

  for (size_t i = 0; i < s_page_row_count; ++i) {
    const char* current_line =
      &s_osd_buffer[i * s_page_col_count];
    const int origin_y =
      PADDING_TOP + (int)i * (font->glyph_height + CHAR_SPACING);

    draw_text(buf, font, PADDING_LEFT, origin_y,
              current_line, CHAR_SPACING);
  }
}

void
previous_log_page(Context* c)
{
  const size_t new_page = c->state.current_log_page - 1;
  c->state.current_log_page = CLAMP(new_page, 1, PAGE_COUNT);
}

void
next_log_page(Context* c)
{
  const size_t new_page = c->state.current_log_page + 1;
  c->state.current_log_page = CLAMP(new_page, 1, PAGE_COUNT);
}

void
deinit_log_buffers()
{
  free(s_log_buffer);
  free(s_osd_buffer);
}
