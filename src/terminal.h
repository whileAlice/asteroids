#pragma once

#include <stdio.h>

typedef enum term_color {
   TERM_BLACK = 0,
   TERM_RED,
   TERM_GREEN,
   TERM_YELLOW,
   TERM_BLUE,
   TERM_MAGENTA,
   TERM_CYAN,
   TERM_WHITE,
   TERM_BRIGHT_BLACK,
   TERM_BRIGHT_RED,
   TERM_BRIGHT_GREEN,
   TERM_BRIGHT_YELLOW,
   TERM_BRIGHT_BLUE,
   TERM_BRIGHT_MAGENTA,
   TERM_BRIGHT_CYAN,
   TERM_BRIGHT_WHITE,
   TERM_DEFAULT,
   TERM_COLOR_COUNT,
} TermColor;

// clang-format off
char* get_term_formatted_string (TermColor fg_color, TermColor bg_color, bool is_bold, const char* str);
void  term_format_begin         (FILE* output, TermColor fg_color, TermColor bg_color, bool is_bold);
void  term_format_end           (FILE* output);
