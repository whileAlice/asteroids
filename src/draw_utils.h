#pragma once

#include <stddef.h>
#include <raylib.h>

/* draws a non-alpha-blended pixel */
void draw_pixel(Image *image, size_t x, size_t y, Color color);

/* draws an alpha-blended-pixel */
void draw_pixel_a(Image *image, size_t x, size_t y, Color color);

/* draws a filled rectangle */
void draw_rectangle_f(Image *image, size_t origin_x, size_t origin_y,
                      size_t width, size_t height, Color color);

/* draws a filled triangle */
void draw_triangle_f(Image *image, int a_x, int a_y, int b_x,
                     int b_y, int c_x, int c_y, Color color);

/* draws an anti-aliased line */
void draw_line(Image *image, size_t start_x, size_t start_y,
               size_t end_x, size_t end_y, Color color);

/* clears the image with supplied color */
void clear_image(Image *image, Color color);

/* calculates index from xy coordinates */
int index_from_xy(Image *image, size_t x, size_t y);
