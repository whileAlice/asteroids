#pragma once

#include <raylib.h>

/* draws a filled rectangle */
void draw_rectangle_fv(Image *image, Vector2 origin,
                       Vector2 size, Color color);

/* draws a filled rectangle with border */
void draw_rectangle_v(Image *image, Vector2 origin, Vector2 size,
                      Color border, Color fill);

/* draws a filled triangle with anti-aliased border */
void draw_triangle_v(Image *image, Vector2 a, Vector2 b,
                     Vector2 c, Color border, Color fill);
