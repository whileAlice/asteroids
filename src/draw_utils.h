#pragma once

#include <stdint.h>
#include <stddef.h>
#include <raylib.h>

typedef struct Color3{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color3;

/* draws a non-alpha-blended pixel */
void draw_pixel(Image *image, int x, int y, Color3 color);

/* draws a non-alpha-blended pixel without bounds checking */
void draw_pixel_unsafe(Image *image, int x, int y, Color3 color);

/* draws an alpha-blended-pixel */
void draw_pixel_a(Image *image, int x, int y, Color color);

/* draws an alpha-blended-pixel without bounds checking */
void draw_pixel_a_unsafe(Image *image, int x, int y, Color color);

/* draws an anti-aliased line (int) */
void draw_line_i(Image *image, int start_x, int start_y,
                 int end_x, int end_y, Color3 color3);

/* draws an anti-aliased line */
void draw_line(Image *image, Vector2 start, Vector2 end, Color3 color);

/* draws a filled rectangle (int) */
void draw_rectangle_fi(Image *image, int origin_x, int origin_y,
                       int width, int height, Color3 color);

/* draws a filled rectangle */
void draw_rectangle_f(Image *image, Vector2 origin,
                      Vector2 size, Color3 color);

/* draws a wireframe rectangle (int) */
void draw_rectangle_wi(Image *image, int origin_x, int origin_y,
                      int width, int height, Color3 color);

/* draws a filled rectangle with border (int) */
void draw_rectangle_i(Image *image, int origin_x, int origin_y, int width,
                      int height, Color3 border, Color3 fill);

/* draws a filled rectangle with border */
void draw_rectangle(Image *image, Vector2 origin, Vector2 size,
                    Color3 border, Color3 fill);

/* draws a filled non-anti-aliased triangle (int) */
void draw_triangle_fi(Image *image, int a_x, int a_y, int b_x,
                      int b_y, int c_x, int c_y, Color3 color);

/* draws a wireframe triangle (int) */
void draw_triangle_wi(Image *image, int a_x, int a_y, int b_x,
                      int b_y, int c_x, int c_y, Color3 color);

/* draws a filled triangle with anti-aliased border (int) */
void draw_triangle_i(Image *image, int a_x, int a_y, int b_x, int b_y,
                     int c_x, int c_y, Color3 border, Color3 fill);

/* draws a filled triangle with anti-aliased border */
void draw_triangle(Image *image, Vector2 a, Vector2 b,
                   Vector2 c, Color3 border, Color3 fill);

/* draws a wireframe quad (int) */
void draw_quad_wi(Image *image, int a_x, int a_y, int b_x, int b_y,
                  int c_x, int c_y, int d_x, int d_y, Color3 color);

/* clears the image with supplied color */
void clear_image(Image *image, Color3 color);

/* calculates index from xy coordinates */
int index_from_xy(Image *image, int x, int y);

/* calculates index from xy coordinates without bounds checking */
size_t index_from_xy_unsafe(Image *image, int x, int y);

/* converts rgba to rgb */
Color3 c3_from_c4(Color color);

/* converts rgb to rgba */
Color c4_from_c3(Color3 color);
