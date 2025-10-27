#include <assert.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "pam.h"
#include "token.h"
#include "draw_utils.h"

#define PAM_HEADER  "P7"
#define PAM_ENDHDR  "ENDHDR"
#define WIDTH       "WIDTH"
#define HEIGHT      "HEIGHT"
#define DEPTH       "DEPTH"
#define MAXVAL      "MAXVAL"
#define TUPLTYPE    "TUPLTYPE"
#define COMMENT     '#'
#define T_RGB       "RGB"
#define T_RGB_ALPHA "RGB_ALPHA"

typedef enum tupltype {
  RGB,
  RGB_ALPHA,
} Tupltype;

// token values
static int      s_width;
static int      s_height;
static int      s_depth;
static int      s_max_val;
static Tupltype s_tupltype;

static int      s_line_count = 0;

Image
image_from_pam(const char *filename)
{
  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("ERROR: couldn't open file: %s\n", filename);
    exit(1);
  }

  char buf[32];
  const char* header = read_next_line(fp, buf, filename, &s_line_count);
  if (strcmp(header, PAM_HEADER) != 0) {
    printf("ERROR: wrong header in file %s; expected: \"%s\", got: \"%s\"\n",
           filename, PAM_HEADER, header);
    exit(1);
  }

  read_all_pam_tokens(fp, buf, filename);
  assert((s_depth = 3 && s_tupltype == RGB) ||
         (s_depth = 4 && s_tupltype == RGB_ALPHA));

  size_t pixel_count = s_width * s_height;
  size_t type_size = s_tupltype == RGB ? sizeof(Color3) : sizeof(Color);
  int format = s_tupltype == RGB ?
               PIXELFORMAT_UNCOMPRESSED_R8G8B8 :
               PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
  void* pixels = (void*)malloc(pixel_count * type_size);
  size_t read_count = fread(pixels, type_size, pixel_count, fp);
  if (read_count < pixel_count) {
    printf(
      "ERROR: mismatch while reading pixels from %s; expected count: %d, got: %d\n",
      filename, (int)pixel_count, (int)read_count
    );
    exit(1);
  }

  // TODO: this could be perhaps a little more elegant?
  if (s_max_val != 255) {
    if (s_tupltype == RGB) {
      Color3* pixels3 = (Color3*)pixels;
      for (size_t i = 0; i < pixel_count; ++i) {
        pixels3[i].r =
          (uint8_t)((pixels3[i].r * 255 + s_max_val / 2) / s_max_val);
        pixels3[i].g =
          (uint8_t)((pixels3[i].g * 255 + s_max_val / 2) / s_max_val);
        pixels3[i].b =
          (uint8_t)((pixels3[i].b * 255 + s_max_val / 2) / s_max_val);
      }
    } else {
      Color* pixels4 = (Color*)pixels;
      for (size_t i = 0; i < pixel_count; ++i) {
        pixels4[i].r =
          (uint8_t)((pixels4[i].r * 255 + s_max_val / 2) / s_max_val);
        pixels4[i].g =
          (uint8_t)((pixels4[i].g * 255 + s_max_val / 2) / s_max_val);
        pixels4[i].b =
          (uint8_t)((pixels4[i].b * 255 + s_max_val / 2) / s_max_val);
        pixels4[i].a =
          (uint8_t)((pixels4[i].a * 255 + s_max_val / 2) / s_max_val);
      }
    }
  }

  if (fclose(fp) == EOF) {
    printf("ERROR: could not close file %s\n", filename);
    exit(1);
  }

  return (Image){
    .data    = pixels,
    .width   = s_width,
    .height  = s_height,
    .mipmaps = 1,
    .format  = format,
  };
}

void
read_all_pam_tokens(FILE* fp, char* buf, const char* filename)
{
  char id[10];
  char tupl_str[10];
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
      if (strcmp(id, WIDTH) == 0) {
        s_width = val;
      } else if (strcmp(id, HEIGHT) == 0) {
        s_height = val;
      } else if (strcmp(id, DEPTH) == 0) {
        s_depth = val;
      } else if (strcmp(id, MAXVAL) == 0) {
        s_max_val = val;
      } else {
        printf("ERROR on line %d in %s: %s is not a valid token identifier\n",
               s_line_count, filename, id);
        exit(1);
      }

      continue;
    case 1:
      if (strcmp(id, TUPLTYPE) == 0) {
        s_line_count--;
        fseek(fp, prev_pos, SEEK_SET);
        fscanf(fp, "%s %s", id, tupl_str);
        if (strcmp(tupl_str, T_RGB) == 0) {
          s_tupltype = RGB;
        } else if (strcmp(tupl_str, T_RGB_ALPHA) == 0) {
          s_tupltype = RGB_ALPHA;
        } else {
          printf("ERROR on line %d in %s: %s is not a valid TUPLTYPE\n",
                 s_line_count, filename, tupl_str);
          exit(1);
        }
      } else if (strcmp(id, PAM_ENDHDR) == 0) {
        return;
      } else {
        printf("ERROR on line %d in %s: %s is not a valid token identifier\n",
               s_line_count, filename, id);
        exit(1);
      }

      continue;
    default:
        printf(
          "ERROR on line %d in %s: unknown error occured while reading tokens\n",
          s_line_count, filename
        );
        exit(1);
    }
  }
}
