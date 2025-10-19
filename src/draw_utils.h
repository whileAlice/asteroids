#pragma once

#include <raylib.h>

/* draws a non-alpha-blended pixel */
void draw_pixel(Image *image, int x, int y, Color color);

/* draws an alpha-blended-pixel */
void draw_pixel_a(Image *image, int x, int y, Color color);

/* draws a filled rectangle */
void draw_rectangle_f(Image *image, int origin_x, int origin_y,
                      int width, int height, Color color);

/* draws a filled triangle */
void draw_triangle_f(Image *image, int a_x, int a_y, int b_x,
                     int b_y, int c_x, int c_y, Color color);

/* draws an anti-aliased line */
void draw_line(Image *image, int start_x, int start_y,
               int end_x, int end_y, Color color);

/* clears the image with supplied color */
void clear_image(Image *image, Color color);

/* calculates index from xy coordinates */
int index_from_xy(Image *image, int x, int y);
