#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "font.h"
#include "pam.h"
#include "token.h"

#define FONTDEF      "FONTDEF"
#define FILENAME     "FILENAME"
#define GLYPH_WIDTH  "GLYPH_WIDTH"
#define GLYPH_HEIGHT "GLYPH_HEIGHT"
#define GLYPH_MARGIN "GLYPH_MARGIN"
#define GLYPH_COUNT  "GLYPH_COUNT"
#define COMMENT      '#'

extern Context g_ctx;

// token values
static char  s_font_filename[32];
static int   s_glyph_width;
static int   s_glyph_height;
static int   s_glyph_margin;
static int   s_glyph_count;

static Image s_glyph_sheet;

static int   s_line_count = 0;

void
load_fixed_font(const char* font_def)
{
  FILE* fp = fopen(font_def, "r");
  if (fp == NULL) {
    printf("ERROR: couldn't open file: %s\n", font_def);
    exit(1);
  }

  char buf[32];
  s_line_count++;
  const char* header = read_next_line(fp, buf, font_def, &s_line_count);
  if (strcmp(header, FONTDEF) != 0) {
    printf("ERROR: wrong header in file %s; expected: \"%s\", got: \"%s\"\n",
           font_def, FONTDEF, header);
    exit(1);
  }

  read_all_font_tokens(fp, buf, font_def);

  if (fclose(fp) == EOF) {
    printf("ERROR: could not close file %s\n", font_def);
    exit(1);
  }

  char font_path[64] = "assets/";
  strcat(font_path, s_font_filename);

  s_glyph_sheet = image_from_pam(font_path);
  // TODO: handle different margin cases
  Margin glyph_margins = { .top    = s_glyph_margin,
                           .bottom = s_glyph_margin,
                           .left   = s_glyph_margin,
                           .right  = s_glyph_margin };

  g_ctx.fixed_font = (FixedFont){
    .glyph_sheet   = &s_glyph_sheet,
    .glyph_padding = glyph_margins,
    .glyph_width   = s_glyph_width,
    .glyph_height  = s_glyph_height,
    .glyph_count   = s_glyph_count,
  };
}

void
unload_fixed_font()
{
  UnloadImage(s_glyph_sheet);
}

void
read_all_font_tokens(FILE* fp, char* buf, const char* filename)
{
  char id[16];
  int  val;

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (ch == COMMENT) {
      read_next_line(fp, buf, filename, &s_line_count);
    } else {
      ungetc(ch, fp);
    }

    s_line_count++;
    long prev_pos = ftell(fp);
    int res = fscanf(fp, "%s %d", id, &val);
    switch (res) {
    case 2:
      if (strcmp(id, GLYPH_WIDTH) == 0) {
        s_glyph_width = val;
      } else if (strcmp(id, GLYPH_HEIGHT) == 0) {
        s_glyph_height = val;
      } else if (strcmp(id, GLYPH_MARGIN) == 0) {
        s_glyph_margin = val;
      } else if (strcmp(id, GLYPH_COUNT) == 0) {
        s_glyph_count = val;
      } else if (strcmp(id, FILENAME) == 0) {
        // TODO: this might need some rethinking
        // since filenames can begin with numbers
        goto filename;
      } else {
        printf("ERROR on line %d in %s: %s is not a valid token identifier\n",
               s_line_count, filename, id);
        exit(1);
      }

      continue;
    case 1:
      if (strcmp(id, FILENAME) == 0) {
filename:
        s_line_count--;
        fseek(fp, prev_pos, SEEK_SET);
        fscanf(fp, "%s %s", id, s_font_filename);
      } else {
        printf("ERROR on line %d in %s: %s is not a valid token identifier\n",
               s_line_count, filename, id);
        exit(1);
      }

      continue;
    default:
      return;
    }
  }
}
