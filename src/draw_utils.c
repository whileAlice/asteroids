#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "math_utils.h"
#include "draw_utils.h"

void
draw_pixel(Image *buf, int x, int y, Color3 color)
{
  int index = index_from_xy(buf, x, y);
  if (index == -1) return;

	Color3 *pixels = (Color3 *)buf->data;
	pixels[index] = color;
}

void
draw_pixel_unsafe(Image *buf, int x, int y, Color3 color)
{
  size_t index = index_from_xy_unsafe(buf, x, y);

	Color3 *pixels = (Color3 *)buf->data;
	pixels[index] = color;
}

static void
internal_draw_pixel_a(Image *buf, int x, int y, Color color, size_t index)
{
  Color3 *pixels = (Color3 *)buf->data;
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
draw_pixel_a(Image *buf, int x, int y, Color color)
{
  int index = index_from_xy(buf, x, y);
  if (index == -1) return;

  internal_draw_pixel_a(buf, x, y, color, index);
}

void
draw_pixel_a_unsafe(Image *buf, int x, int y, Color color)
{
  size_t index = index_from_xy_unsafe(buf, x, y);

  internal_draw_pixel_a(buf, x, y, color, index);
}

void
draw_image(Image *buf, Image *img, int origin_x, int origin_y)
{
  const int start_x = CLAMP(origin_x, 0, buf->width  - 1);
  const int start_y = CLAMP(origin_y, 0, buf->height - 1);
  const int end_x = CLAMPR(start_x + img->width  - 1, buf->width  - 1);
  const int end_y = CLAMPR(start_y + img->height - 1, buf->height - 1);

  Color3 *pixels_buf = (Color3 *)buf->data;
  // TODO: should be Color3 in img data
  Color  *pixels_img = (Color  *)img->data;
  int img_y, img_x = 0;

  // TODO: this looks like it could be optimized?
  for (int buf_x = start_x; buf_x <= end_x; ++buf_x) {
    img_y = 0;

    for (int buf_y = start_y; buf_y <= end_y; ++buf_y) {
      size_t index_buf = index_from_xy_unsafe(buf, buf_x, buf_y);
      size_t index_img = index_from_xy_unsafe(img, img_x, img_y);

      pixels_buf[index_buf].r = pixels_img[index_img].r;
      pixels_buf[index_buf].g = pixels_img[index_img].g;
      pixels_buf[index_buf].b = pixels_img[index_img].b;

      img_y++;
    }

    img_x++;
  }
}

void
draw_line_i(Image *buf, int start_x, int start_y,
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
      draw_pixel_a(buf, (int)(intersect_y) + 1, i, color);

      color.a = (uint8_t)(255.f * (1.0f - fractional_part(intersect_y)));
      draw_pixel_a(buf, (int)(intersect_y), i, color);

      intersect_y += gradient;
    }
  } else {
    for (int i = start_x; i <= end_x; ++i) {
      color.a = (uint8_t)(255.f * fractional_part(intersect_y));
      draw_pixel_a(buf, i, (int)(intersect_y) + 1, color);

      color.a = (uint8_t)(255.f * (1.0f - fractional_part(intersect_y)));
      draw_pixel_a(buf, i, (int)(intersect_y), color);

      intersect_y += gradient;
    }
  }
}

void
draw_line(Image *buf, Vector2 start, Vector2 end, Color3 color)
{
  draw_line_i(buf,
              (int)roundf(start.x), (int)roundf(start.y),
              (int)roundf(end.x),   (int)roundf(end.y),
              color);
}

void
draw_rectangle_fi(Image *buf, int origin_x, int origin_y,
                  int width, int height, Color3 color)
{
  if (width == 0 || height == 0) return;

  const int start_x = CLAMP(origin_x, 0, buf->width  - 1);
  const int start_y = CLAMP(origin_y, 0, buf->height - 1);
  const int end_x = CLAMPR(start_x + width  - 1, buf->width  - 1);
  const int end_y = CLAMPR(start_y + height - 1, buf->height - 1);

  for (int x = start_x; x <= end_x; ++x) {
    for (int y = start_y; y <= end_y; ++y) {
      draw_pixel_unsafe(buf, x, y, color);
    }
  }
}

void
draw_rectangle_f(Image *buf, Vector2 origin,
                 Vector2 size, Color3 color)
{
  draw_rectangle_fi(buf,
                    (int)roundf(origin.x), (int)roundf(origin.y),
                    (int)roundf(size.x),   (int)roundf(size.y),
                    color);
}


void
draw_rectangle_wi(Image *buf, int origin_x, int origin_y,
                  int width, int height, Color3 color)
{
  draw_quad_wi(buf,
               origin_x,         origin_y,
               origin_x + width, origin_y,
               origin_x + width, origin_y + height,
               origin_x,         origin_y + height,
               color);
}

void
draw_rectangle_i(Image *buf, int origin_x, int origin_y, int width,
                 int height, Color3 border, Color3 fill)
{
  draw_rectangle_fi(buf, origin_x, origin_y, width, height, fill);
  draw_rectangle_wi(buf, origin_x, origin_y, width, height, border);
}

void
draw_rectangle(Image *buf, Vector2 origin, Vector2 size,
               Color3 border, Color3 fill)
{
  draw_rectangle_i(buf,
                   (int)roundf(origin.x), (int)roundf(origin.y),
                   (int)roundf(size.x),   (int)roundf(size.y),
                   border, fill);
}

void
draw_triangle_fi(Image *buf, int a_x, int a_y, int b_x,
                 int b_y, int c_x, int c_y, Color3 color)
{
  // TODO: convert to scanline-based approach
  if (!is_clockwise(a_x, a_y, b_x, b_y, c_x, c_y)) {
    SWAP(a_x, b_x);
    SWAP(a_y, b_y);
  }

  const int min_x = CLAMP(MIN(MIN(a_x, b_x), c_x), 0, buf->width  - 1);
  const int min_y = CLAMP(MIN(MIN(a_y, b_y), c_y), 0, buf->height - 1);
  const int max_x = CLAMP(MAX(MAX(a_x, b_x), c_x), 0, buf->width  - 1);
  const int max_y = CLAMP(MAX(MAX(a_y, b_y), c_y), 0, buf->height - 1);

  for (int x = min_x; x <= max_x; ++x) {
    for (int y = min_y; y <= max_y; ++y) {
      if (
        is_clockwise(a_x, a_y, b_x, b_y, x, y) &&
        is_clockwise(b_x, b_y, c_x, c_y, x, y) &&
        is_clockwise(c_x, c_y, a_x, a_y, x, y)
      ) {
          draw_pixel_unsafe(buf, x, y, color);
        }
    }
  }
}

void
draw_triangle_wi(Image *buf, int a_x, int a_y, int b_x,
                 int b_y, int c_x, int c_y, Color3 color)
{
  draw_line_i(buf, a_x, a_y, b_x, b_y, color);
  draw_line_i(buf, b_x, b_y, c_x, c_y, color);
  draw_line_i(buf, c_x, c_y, a_x, a_y, color);
}

void
draw_triangle_i(Image *buf, int a_x, int a_y, int b_x, int b_y,
                int c_x, int c_y, Color3 border, Color3 fill)
{
  draw_triangle_fi(buf, a_x, a_y, b_x, b_y, c_x, c_y, fill);
  draw_triangle_wi(buf, a_x, a_y, b_x, b_y, c_x, c_y, border);
}

void
draw_triangle(Image *buf, Vector2 a, Vector2 b,
              Vector2 c, Color3 border, Color3 fill)
{
  draw_triangle_i(buf,
                  (int)roundf(a.x), (int)roundf(a.y),
                  (int)roundf(b.x), (int)roundf(b.y),
                  (int)roundf(c.x), (int)roundf(c.y),
                  border, fill);
}

void
draw_quad_wi(Image *buf, int a_x, int a_y, int b_x, int b_y,
             int c_x, int c_y, int d_x, int d_y, Color3 color)
{
  draw_line_i(buf, a_x, a_y, b_x, b_y, color);
  draw_line_i(buf, b_x, b_y, c_x, c_y, color);
  draw_line_i(buf, c_x, c_y, d_x, d_y, color);
  draw_line_i(buf, d_x, d_y, a_x, a_y, color);
}

void
clear_image(Image *img, Color3 color)
{
  Color3 *addr = img->data;
  size_t count = img->width * img->height;

  while (count--) *addr++ = color;
}

int
index_from_xy(Image *img, int x, int y)
{
  const int index = img->width * y + x;
  const int max_index = img->width * img->height - 1;
  assert(max_index >= 0);

	return index >= 0 && index <= max_index ? index : -1;
}

size_t
index_from_xy_unsafe(Image *img, int x, int y)
{
  return img->width * y + x;
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
