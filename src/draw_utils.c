#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "math_utils.h"
#include "draw_utils.h"

void
draw_pixel(Image *image, size_t x, size_t y, Color color)
{
  assert(color.a == 255);

  int index = index_from_xy(image, x, y);
  if (index == -1) return;

	Color *pixels = (Color *)image->data;

	pixels[index] = color;
}

void
draw_pixel_a(Image *image, size_t x, size_t y, Color color)
{
  int index = index_from_xy(image, x, y);
  if (index == -1) return;

  Color *pixels = (Color *)image->data;

  if (color.a < 255) {
    float alpha_mul = (float)color.a / 255.0f;

    Color bg = pixels[index];
    bg.r = (unsigned char)((float)bg.r * (1 - alpha_mul) + 0.5f);
    bg.g = (unsigned char)((float)bg.g * (1 - alpha_mul) + 0.5f);
    bg.b = (unsigned char)((float)bg.b * (1 - alpha_mul) + 0.5f);

    color.r = (unsigned char)((float)color.r * alpha_mul + 0.5f) + bg.r;
    color.g = (unsigned char)((float)color.g * alpha_mul + 0.5f) + bg.g;
    color.b = (unsigned char)((float)color.b * alpha_mul + 0.5f) + bg.b;
    color.a = 255;
  }

  pixels[index] = color;
}

void
draw_rectangle_f(Image *image, size_t origin_x, size_t origin_y,
                 size_t width, size_t height, Color color)
{
  for (size_t x = origin_x; x < width; ++x) {
    for (size_t y = origin_y; y < height; ++y) {
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
draw_line(Image *image, size_t start_x, size_t start_y,
          size_t end_x, size_t end_y, Color color)
{
  bool is_steep = abs((int)end_y - (int)start_y) > abs((int)end_x - (int)start_x);

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
    for (size_t i = start_x; i <= end_x; ++i) {
      color.a = (unsigned char)(255.f * fractional_part(intersect_y));
      draw_pixel_a(image, (size_t)(intersect_y) + 1, i, color);

      color.a = (unsigned char)(255.f * (1.0f - fractional_part(intersect_y)));
      draw_pixel_a(image, (size_t)(intersect_y), i, color);

      intersect_y += gradient;
    }
  } else {
    for (size_t i = start_x; i <= end_x; ++i) {
      color.a = (unsigned char)(255.f * fractional_part(intersect_y));
      draw_pixel_a(image, i, (size_t)(intersect_y) + 1, color);

      color.a = (unsigned char)(255.f * (1.0f - fractional_part(intersect_y)));
      draw_pixel_a(image, i, (size_t)(intersect_y), color);

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
index_from_xy(Image *image, size_t x, size_t y)
{
  const int index = image->width * (int)y + (int)x;
  const int max_index = image->width * image->height - 1;
  assert(max_index >= 0);

	return index >= 0 && index <= max_index ? index : -1;
}
