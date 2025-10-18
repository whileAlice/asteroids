#pragma once

#include <stddef.h>
#include <raylib.h>

void draw_pixel(Image *image, size_t x, size_t y, Color color);
void draw_rectangle(Image *image, size_t origin_x, size_t origin_y,
                    size_t width, size_t height, Color color);
void draw_triangle(Image *image, size_t a_x, size_t a_y, size_t b_x,
                   size_t b_y, size_t c_x, size_t c_y, Color color);
void draw_line(Image *image, size_t start_x, size_t start_y,
               size_t end_x, size_t end_y, Color color);
void clear_image(Image *image, Color color);
