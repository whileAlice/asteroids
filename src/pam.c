#include <assert.h>
#include <ctype.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pam.h"

#define PAM_HEADER "P7"
#define PAM_ENDHDR "ENDHDR"
#define TUPLTYPE   "TUPLTYPE"
#define RGB        "RGB"
#define RGB_ALPHA  "RGB_ALPHA"

// TODO: handle tokens properly
Image
image_from_pam(const char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("ERROR: couldn't open file: %s\n", filename);
    exit(1);
  }

  char line[32];

  const char *header = read_next_line(fp, line, filename, "header");
  if (strcmp(PAM_HEADER, header) != 0) {
    printf("ERROR: wrong file header for filename: %s; expected: %s, got: %s\n",
           filename, PAM_HEADER, header);
  }

  const int width = get_next_value(fp, line, filename, "width");
  const int height = get_next_value(fp, line, filename, "height");
  const int depth = get_next_value(fp, line, filename, "depth");
  if (depth != 3 && depth != 4) {
    printf("ERROR: invalid depth %d in file: %s\n", depth, filename);
    exit(1);
  }
  const int max_val = get_next_value(fp, line, filename, "maximum value");
  if (max_val > 255) {
    printf("ERROR: invalid maximum value %d in file: %s\n", max_val, filename);
    exit(1);
  }

  float scale_by = 1.0f;
  if (max_val != 255) {
    scale_by = (float)max_val / 255.0f;
  }

  char *tupltype = read_next_line(fp, line, filename, "TUPLTYPE tokens");
  const char *tupltype_id = strtok(tupltype, " ");
  if (tupltype_id == NULL) {
    printf("ERROR: couldn't extract %s identifier from file: %s\n", TUPLTYPE, filename);
    exit(1);
  }

  if (strcmp(tupltype_id, TUPLTYPE) != 0) {
    printf("ERROR: %s identifier not found in file: %s; got: %s\n",
           TUPLTYPE, filename, tupltype_id);
    exit(1);
  }

  const char *tupltype_val = strtok(NULL, " ");
  if (tupltype_val == NULL) {
    printf("ERROR: couldn't extract %s value from file: %s\n", TUPLTYPE, filename);
    exit(1);
  }

  // TODO: connect depth check to TUPLTYPE value check
  if (strcmp(tupltype_val, "RGB") != 0 ||
      strcmp(tupltype_val, "RGB_ALPHA") != 0) {
    printf("ERROR: expected %s value not found in file: %s; got: %s\n",
           TUPLTYPE, filename, tupltype_val);
    exit(1);
  }

  char *end_header = read_next_line(fp, line, filename, PAM_ENDHDR);
  if (strcmp(end_header, PAM_ENDHDR) != 0) {
    printf("ERROR: %s identifier not found in file: %s; got: %s\n",
           PAM_ENDHDR, filename, end_header);
    exit(1);
  }

  // TODO: handle other cases
  assert(strcmp(tupltype_val, RGB_ALPHA) == 0 && depth == 4 && max_val == 255);

  size_t size = width * height;
  Color *pixels = (Color *)malloc(size * sizeof(Color));

  size_t read = fread(pixels, size, sizeof(Color), fp);
  if (read < size) {
    printf("ERROR: error while reading: %s; expected size: %d, got: %d\n",
           filename, (int)size, (int)read);
    exit(1);
  }

  return (Image){
    .data    = pixels,
    .width   = width,
    .height  = height,
    .mipmaps = 1,
    .format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };
}

char
*read_next_line(FILE *fp, char *buffer, const char *filename, const char *name)
{
  char *line = fgets(buffer, sizeof(buffer), fp);
  if (line == NULL) {
    printf("ERROR: couldn't read %s from file: %s\n", name, filename);
    exit(1);
  }

  return line;
}

void
seek_to_space(FILE *fp)
{
  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (isspace(ch)) break;
  }
}

int
get_next_value(FILE *fp, char *buffer, const char *filename, const char *name)
{
  seek_to_space(fp);
  const char *value_s = read_next_line(fp, buffer, filename, name);
  const int value = atoi(value_s);
  if (value <= 0) {
    printf("ERROR: invalid %s %d in file: %s\n", name, value, filename);
    exit(1);
  }

  return value;
}
