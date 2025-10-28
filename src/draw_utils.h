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
void draw_pixel(Image* buf, int x, int y, Color3 color);

/* draws a non-alpha-blended pixel without bounds checking */
void draw_pixel_unsafe(Image* buf, int x, int y, Color3 color);

/* draws an alpha-blended-pixel */
void draw_pixel_a(Image* buf, int x, int y, Color color);

/* draws an alpha-blended-pixel without bounds checking */
void draw_pixel_a_unsafe(Image* buf, int x, int y, Color color);

/* draws a non-alpha-blended image */
void draw_image(Image* buf, Image* img, int origin_x, int origin_y);

/* draws an alpha-blended image */
void draw_image_a(Image* buf, Image* img, int origin_x, int origin_y);

/* draws a glyph using the given font */
void draw_glyph(Image* buf, FixedFont* font, int origin_x,
                int origin_y, size_t index);

/* draws text using the given font */
void draw_text(Image* buf, FixedFont* font, int origin_x,
               int origin_y, const char* text, int padding);

/* draws an anti-aliased line (int) */
void draw_line_i(Image* buf, int start_x, int start_y,
                 int end_x, int end_y, Color color);

/* draws an anti-aliased line */
void draw_line(Image* buf, Vector2 start, Vector2 end, Color color);

/* draws a filled rectangle (int) */
void draw_rectangle_fi(Image* buf, int origin_x, int origin_y,
                       int width, int height, Color color);

/* draws a filled rectangle */
void draw_rectangle_f(Image* buf, Vector2 origin,
                      Vector2 size, Color color);

/* draws a wireframe rectangle (int) */
void draw_rectangle_wi(Image* buf, int origin_x, int origin_y,
                       int width, int height, Color color);

/* draws a filled rectangle with border (int) */
void draw_rectangle_i(Image* buf, int origin_x, int origin_y, int width,
                      int height, Color border, Color fill);

/* draws a filled rectangle with border */
void draw_rectangle(Image* buf, Vector2 origin, Vector2 size,
                    Color border, Color fill);

/* draws a filled non-anti-aliased triangle (int) */
void draw_triangle_fi(Image* buf, int a_x, int a_y, int b_x,
                      int b_y, int c_x, int c_y, Color color);

/* draws a wireframe triangle (int) */
void draw_triangle_wi(Image* buf, int a_x, int a_y, int b_x,
                      int b_y, int c_x, int c_y, Color color);

/* draws a filled triangle with anti-aliased border (int) */
void draw_triangle_i(Image* buf, int a_x, int a_y, int b_x, int b_y,
                     int c_x, int c_y, Color border, Color fill);

/* draws a filled triangle with anti-aliased border */
void draw_triangle(Image* buf, Vector2 a, Vector2 b,
                   Vector2 c, Color border, Color fill);

/* draws a wireframe quad (int) */
void draw_quad_wi(Image* buf, int a_x, int a_y, int b_x, int b_y,
                  int c_x, int c_y, int d_x, int d_y, Color color);

/* blurs an image using box blur */
void box_blur(Image* dst, Image* src);

/* clears the image with supplied RGB color */
void clear_image_rgb(Image* img, Color3 color);

/* makes a clone of the given image */
Image clone_image(Image src);

/* calculates index from xy coordinates */
int index_from_xy(Image* img, int x, int y);

/* calculates index from xy coordinates without bounds checking */
size_t index_from_xy_unsafe(Image* img, int x, int y);

/* converts rgba to rgb */
Color3 rgb_from_rgba(Color color);

/* converts rgb to rgba */
Color rgba_from_rgb(Color3 color);

/* alpha blends bg with fg */
void blend_bg_with_fg(Color3* bg, Color* fg);
