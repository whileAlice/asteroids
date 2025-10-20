#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "math_utils.h"
#include "draw_utils.h"

void
draw_pixel(Image *image, int x, int y, Color3 color)
{
  int index = index_from_xy(image, x, y);
  if (index == -1) return;

	Color3 *pixels = (Color3 *)image->data;

	pixels[index] = color;
}

void
draw_pixel_a(Image *image, int x, int y, Color color)
{
  int index = index_from_xy(image, x, y);
  if (index == -1) return;

  Color3 *pixels = (Color3 *)image->data;
  uint8_t a = color.a;

  switch (a) {
  case 0:
    return;
  case 255:
    pixels[index].r = color.r;
    pixels[index].g = color.g;
    pixels[index].b = color.b;
    return;
  default:
    pixels[index].r = (pixels[index].r * (255 - a) + color.r * a + 128) / 255;
    pixels[index].g = (pixels[index].g * (255 - a) + color.g * a + 128) / 255;
    pixels[index].b = (pixels[index].b * (255 - a) + color.b * a + 128) / 255;
  }
}

void
draw_line_i(Image *image, int start_x, int start_y,
            int end_x, int end_y, Color3 color3)
{
  Color color = c4_from_c3(color3);
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
draw_line(Image *image, Vector2 start, Vector2 end, Color3 color)
{
  draw_line_i(image,
              (int)roundf(start.x), (int)roundf(start.y),
              (int)roundf(end.x),   (int)roundf(end.y),
              color);
}

void
draw_rectangle_fi(Image *image, int origin_x, int origin_y,
                  int width, int height, Color3 color)
{
  for (int x = origin_x; x < width; ++x) {
    for (int y = origin_y; y < height; ++y) {
      draw_pixel(image, x, y, color);
    }
  }
}

void
draw_rectangle_f(Image *image, Vector2 origin,
                 Vector2 size, Color3 color)
{
  draw_rectangle_fi(image,
                    (int)roundf(origin.x), (int)roundf(origin.y),
                    (int)roundf(size.x),   (int)roundf(size.y),
                    color);
}


void
draw_rectangle_wi(Image *image, int origin_x, int origin_y,
                  int width, int height, Color3 color)
{
  draw_quad_wi(image,
               origin_x,         origin_y,
               origin_x + width, origin_y,
               origin_x + width, origin_y + height,
               origin_x,         origin_y + height,
               color);
}

void
draw_rectangle_i(Image *image, int origin_x, int origin_y, int width,
                 int height, Color3 border, Color3 fill)
{
  draw_rectangle_fi(image, origin_x, origin_y, width, height, fill);
  draw_rectangle_wi(image, origin_x, origin_y, width, height, border);
}

void
draw_rectangle(Image *image, Vector2 origin, Vector2 size,
               Color3 border, Color3 fill)
{
  draw_rectangle_i(image,
                   (int)roundf(origin.x), (int)roundf(origin.y),
                   (int)roundf(size.x),   (int)roundf(size.y),
                   border, fill);
}

void
draw_triangle_fi(Image *image, int a_x, int a_y, int b_x,
                 int b_y, int c_x, int c_y, Color3 color)
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
}

void
draw_triangle_wi(Image *image, int a_x, int a_y, int b_x,
                 int b_y, int c_x, int c_y, Color3 color)
{
  draw_line_i(image, a_x, a_y, b_x, b_y, color);
  draw_line_i(image, b_x, b_y, c_x, c_y, color);
  draw_line_i(image, c_x, c_y, a_x, a_y, color);
}

void
draw_triangle_i(Image *image, int a_x, int a_y, int b_x, int b_y,
                int c_x, int c_y, Color3 border, Color3 fill)
{
  draw_triangle_fi(image, a_x, a_y, b_x, b_y, c_x, c_y, fill);
  draw_triangle_wi(image, a_x, a_y, b_x, b_y, c_x, c_y, border);
}

void
draw_triangle(Image *image, Vector2 a, Vector2 b,
              Vector2 c, Color3 border, Color3 fill)
{
  draw_triangle_i(image,
                  (int)roundf(a.x), (int)roundf(a.y),
                  (int)roundf(b.x), (int)roundf(b.y),
                  (int)roundf(c.x), (int)roundf(c.y),
                  border, fill);
}

void
draw_quad_wi(Image *image, int a_x, int a_y, int b_x, int b_y,
             int c_x, int c_y, int d_x, int d_y, Color3 color)
{
  draw_line_i(image, a_x, a_y, b_x, b_y, color);
  draw_line_i(image, b_x, b_y, c_x, c_y, color);
  draw_line_i(image, c_x, c_y, d_x, d_y, color);
  draw_line_i(image, d_x, d_y, a_x, a_y, color);
}

void
clear_image(Image *image, Color3 color)
{
  Color3 *addr = image->data;
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

Color3
c3_from_c4(Color color)
{
  typedef union {
    Color  c4;
    Color3 c3;
  } converter;

  converter c;
  c.c4 = color;

  return c.c3;
}

Color
c4_from_c3(Color3 color)
{
  return (Color){
    .r = color.r,
    .g = color.g,
    .b = color.b,
    .a = 255,
  };
}
