#pragma once

#include <stdint.h>
#include <stddef.h>
#include <raylib.h>

#include "font.h"

typedef struct Color3{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color3;

/* draws a non-alpha-blended pixel */
void draw_rgb_pixel(Image* buf, int x, int y, Color3 pixel);

/* draws a non-alpha-blended pixel without bounds checking */
void draw_rgb_pixel_unsafe(Image* buf, int x, int y, Color3 pixel);

/* draws an alpha-blended-pixel */
void draw_rgba_pixel(Image* buf, int x, int y, Color pixel);

/* draws an alpha-blended-pixel without bounds checking */
void draw_rgba_pixel_unsafe(Image* buf, int x, int y, Color pixel);

/* blends an rgba pixel onto a rgb buffer */
static void blend_rgba_pixel_on_rgb_buffer(Image* buf, int x, int y,
                                           Color pixel, size_t index);

/* draws a non-alpha-blended image */
void draw_rgb_image(Image* buf, Image* img, int origin_x, int origin_y);

/* draws an alpha-blended image */
void draw_rgba_image(Image* buf, Image* img, int origin_x, int origin_y);

/* draws a glyph using the given font */
void draw_glyph(Image* buf, FixedFont* font, int origin_x,
                int origin_y, size_t index);

/* draws text using the given font */
void draw_text(Image* buf, FixedFont* font, int origin_x,
               int origin_y, const char* text, int padding);

/* draws an anti-aliased line (int) */
void draw_line_i(Image* buf, int start_x, int start_y,
                 int end_x, int end_y, Color pixel);

/* draws an anti-aliased line */
void draw_line(Image* buf, Vector2 start, Vector2 end, Color pixel);

/* draws a filled rectangle (int) */
void draw_rectangle_fi(Image* buf, int origin_x, int origin_y,
                       int width, int height, Color pixel);

/* draws a filled rectangle */
void draw_rectangle_f(Image* buf, Vector2 origin,
                      Vector2 size, Color pixel);

/* draws a wireframe rectangle (int) */
void draw_rectangle_wi(Image* buf, int origin_x, int origin_y,
                       int width, int height, Color pixel);

/* draws a filled rectangle with border (int) */
void draw_rectangle_i(Image* buf, int origin_x, int origin_y, int width,
                      int height, Color border, Color fill);

/* draws a filled rectangle with border */
void draw_rectangle(Image* buf, Vector2 origin, Vector2 size,
                    Color border, Color fill);

/* draws a filled non-anti-aliased triangle (int) */
void draw_triangle_fi(Image* buf, int a_x, int a_y, int b_x,
                      int b_y, int c_x, int c_y, Color pixel);

/* draws a wireframe triangle (int) */
void draw_triangle_wi(Image* buf, int a_x, int a_y, int b_x,
                      int b_y, int c_x, int c_y, Color pixel);

/* draws a filled triangle with anti-aliased border (int) */
void draw_triangle_i(Image* buf, int a_x, int a_y, int b_x, int b_y,
                     int c_x, int c_y, Color border, Color fill);

/* draws a filled triangle with anti-aliased border */
void draw_triangle(Image* buf, Vector2 a, Vector2 b,
                   Vector2 c, Color border, Color fill);

/* draws a wireframe quad (int) */
void draw_quad_wi(Image* buf, int a_x, int a_y, int b_x, int b_y,
                  int c_x, int c_y, int d_x, int d_y, Color pixel);

/* blurs an image using box blur */
void box_blur(Image* dst, Image* src, size_t iterations);

/* brightens or darkens an image by specified amount (0-255) */
void brighten_image_by_amount(Image* dst, Image* src, int amount);

/* brightens or darkens an image by relative percentage */
void brighten_image_by_percentage(Image* dst, Image* src, int percentage);

/* clears the image with supplied RGB pixel */
void clear_rgb_image(Image* img, Color3 pixel);

/* makes a clone of the given image */
Image clone_image(Image* src);

/* calculates index from xy coordinates */
int index_from_xy(Image* img, int x, int y);

/* calculates index from xy coordinates without bounds checking */
size_t index_from_xy_unsafe(Image* img, int x, int y);

/* converts rgba to rgb */
Color3 rgb_from_rgba(Color pixel);

/* converts rgb to rgba */
Color rgba_from_rgb(Color3 pixel);

/* alpha blends bg with fg */
void blend_rgba_pixel_on_rgb_pixel(Color3* bg, Color* fg);
