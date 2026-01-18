#include "terminal.h"

#include "error.h"

#include <stdlib.h>

#define TERM_FORMAT_START        "\033["
#define TERM_FORMAT_RESET        TERM_FORMAT_START "0m"
#define TERM_FORMAT_WITH_BOLD    "%s%s;%s;1m%s%s"
#define TERM_FORMAT_WITHOUT_BOLD "%s%s;%sm%s%s"

static const char* s_fg_codes[] = {
   [TERM_BLACK] = "30",        [TERM_RED] = "31",
   [TERM_GREEN] = "32",        [TERM_YELLOW] = "33",
   [TERM_BLUE] = "34",         [TERM_MAGENTA] = "35",
   [TERM_CYAN] = "36",         [TERM_WHITE] = "37",
   [TERM_BRIGHT_BLACK] = "90", [TERM_BRIGHT_RED] = "91",
   [TERM_BRIGHT_GREEN] = "92", [TERM_BRIGHT_YELLOW] = "93",
   [TERM_BRIGHT_BLUE] = "94",  [TERM_BRIGHT_MAGENTA] = "95",
   [TERM_BRIGHT_CYAN] = "96",  [TERM_BRIGHT_WHITE] = "97",
   [TERM_DEFAULT] = "39",
};

static const char* s_bg_codes[] = {
   [TERM_BLACK] = "40",         [TERM_RED] = "41",
   [TERM_GREEN] = "42",         [TERM_YELLOW] = "43",
   [TERM_BLUE] = "44",          [TERM_MAGENTA] = "45",
   [TERM_CYAN] = "46",          [TERM_WHITE] = "47",
   [TERM_BRIGHT_BLACK] = "100", [TERM_BRIGHT_RED] = "101",
   [TERM_BRIGHT_GREEN] = "102", [TERM_BRIGHT_YELLOW] = "103",
   [TERM_BRIGHT_BLUE] = "104",  [TERM_BRIGHT_MAGENTA] = "105",
   [TERM_BRIGHT_CYAN] = "106",  [TERM_BRIGHT_WHITE] = "107",
   [TERM_DEFAULT] = "49",
};

// TODO: this should be also preferably integrated with
// the various string assemblies in log.c
char*
get_term_formatted_string (TermColor fg_color, TermColor bg_color, bool is_bold,
                           const char* str)
{
   const char* format =
      is_bold ? TERM_FORMAT_WITH_BOLD : TERM_FORMAT_WITHOUT_BOLD;

   int length =
      snprintf (NULL, 0, format, TERM_FORMAT_START, s_fg_codes[fg_color],
                s_bg_codes[bg_color], str, TERM_FORMAT_RESET);
   if (length < 0)
      ERRNO_RETURN (NULL, "null snprintf");

   size_t formatted_str_size = length * sizeof (char) + 1;
   char*  formatted_str      = malloc (formatted_str_size);
   length = snprintf (formatted_str, formatted_str_size, format,
                      TERM_FORMAT_START, s_fg_codes[fg_color],
                      s_bg_codes[bg_color], str, TERM_FORMAT_RESET);
   if (length < 0)
      ERRNO_RETURN (NULL, "formatted str snprintf");

   return formatted_str;
}

void
term_format_begin (FILE* output, TermColor fg_color, TermColor bg_color,
                   bool is_bold)
{
   fputs ("\033[", output);

   fputs (s_fg_codes[fg_color], output);
   fputs (";", output);
   fputs (s_bg_codes[bg_color], output);

   if (is_bold)
      fputs (";1", output);

   fputs ("m", output);
}

void
term_format_end (FILE* output)
{
   fputs ("\033[0m", output);
}
