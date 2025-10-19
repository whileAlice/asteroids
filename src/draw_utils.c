#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "math_utils.h"
#include "draw_utils.h"

void
draw_pixel(Image *image, int x, int y, Color color)
{
  assert(color.a == 255);

  int index = index_from_xy(image, x, y);
  if (index == -1) return;

	Color *pixels = (Color *)image->data;

	pixels[index] = color;
}

void
draw_pixel_a(Image *image, int x, int y, Color color)
{
  int index = index_from_xy(image, x, y);
  if (index == -1) return;

  Color *pixels = (Color *)image->data;

  if (color.a < 255) {
    float alpha_mul = (float)color.a / 255.0f;

    Color bg = pixels[index];
    bg.r = ROUND((float)bg.r * (1 - alpha_mul), uint8_t);
    bg.g = ROUND((float)bg.g * (1 - alpha_mul), uint8_t);
    bg.b = ROUND((float)bg.b * (1 - alpha_mul), uint8_t);

    color.r = ROUND((float)color.r * alpha_mul, uint8_t) + bg.r;
    color.g = ROUND((float)color.g * alpha_mul, uint8_t) + bg.g;
    color.b = ROUND((float)color.b * alpha_mul, uint8_t) + bg.b;
    color.a = 255;
  }

  pixels[index] = color;
}

void
draw_rectangle_f(Image *image, int origin_x, int origin_y,
                 int width, int height, Color color)
{
  for (int x = origin_x; x < width; ++x) {
    for (int y = origin_y; y < height; ++y) {
      draw_pixel(image, x, y, color);
    }
  }
}

void
draw_triangle_f(Image *image, int a_x, int a_y, int b_x,
                int b_y, int c_x, int c_y, Color color)
{
  if (!is_clockwise(a_x, a_y, b_x, b_y, c_x, c_y)) {
    SWAP(a_x, b_x);
    SWAP(a_y, b_y);
  }

  const int min_x = MIN(MIN(a_x, b_x), c_x);
  const int min_y = MIN(MIN(a_y, b_y), c_y);
  const int max_x = MAX(MAX(a_x, b_x), c_x);
  const int max_y = MAX(MAX(a_y, b_y), c_y);

  for (int x = min_x; x <= max_x; ++x) {
    for (int y = min_y; y <= max_y; ++y) {
      if (
        is_clockwise(a_x, a_y, b_x, b_y, x, y) &&
        is_clockwise(b_x, b_y, c_x, c_y, x, y) &&
        is_clockwise(c_x, c_y, a_x, a_y, x, y)
      ) {
          draw_pixel(image, x, y, color);
        }
    }
  }

  draw_line(image, a_x, a_y, b_x, b_y, color);
  draw_line(image, b_x, b_y, c_x, c_y, color);
  draw_line(image, c_x, c_y, a_x, a_y, color);
}

void
draw_line(Image *image, int start_x, int start_y,
          int end_x, int end_y, Color color)
{
  bool is_steep = abs(end_y - start_y) > abs(end_x - start_x);

  if (is_steep) {
    SWAP(start_x, start_y);
    SWAP(end_x, end_y);
  }

  if (start_x > end_x) {
    SWAP(start_x, end_x);
    SWAP(start_y, end_y);
  }

  float dx = (float)end_x - (float)start_x;
  float dy = (float)end_y - (float)start_y;

  float gradient = dx != 0.0f ? dy / dx : 1.0f;
  float intersect_y = (float)start_y;

  if (is_steep) {
    for (int i = start_x; i <= end_x; ++i) {
      color.a = (uint8_t)(255.f * fractional_part(intersect_y));
      draw_pixel_a(image, (int)(intersect_y) + 1, i, color);

      color.a = (uint8_t)(255.f * (1.0f - fractional_part(intersect_y)));
      draw_pixel_a(image, (int)(intersect_y), i, color);

      intersect_y += gradient;
    }
  } else {
    for (int i = start_x; i <= end_x; ++i) {
      color.a = (uint8_t)(255.f * fractional_part(intersect_y));
      draw_pixel_a(image, i, (int)(intersect_y) + 1, color);

      color.a = (uint8_t)(255.f * (1.0f - fractional_part(intersect_y)));
      draw_pixel_a(image, i, (int)(intersect_y), color);

      intersect_y += gradient;
    }
  }
}

void
clear_image(Image *image, Color color)
{
  Color *addr = image->data;
  size_t count = image->width * image->height;

  while (count--) *addr++ = color;
}

int
index_from_xy(Image *image, int x, int y)
{
  const int index = image->width * y + x;
  const int max_index = image->width * image->height - 1;
  assert(max_index >= 0);

	return index >= 0 && index <= max_index ? index : -1;
}
