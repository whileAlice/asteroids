#include <raylib.h>

#include "draw_utils_vec.h"
#include "draw_utils.h"
#include "math_utils.h"

void
draw_rectangle_fv(Image *image, Vector2 origin,
                  Vector2 size, Color color)
{
  draw_rectangle_f(image,
                   ROUND(origin.x, int), ROUND(origin.y, int),
                   ROUND(size.x, int),   ROUND(size.y, int),
                   color);
}

void
draw_rectangle_v(Image *image, Vector2 origin, Vector2 size,
                 Color border, Color fill)
{
  draw_rectangle(image,
                 ROUND(origin.x, int), ROUND(origin.y, int),
                 ROUND(size.x, int),   ROUND(size.y, int),
                 border, fill);
}

void
draw_triangle_v(Image *image, Vector2 a, Vector2 b,
                Vector2 c, Color border, Color fill)
{
  draw_triangle(image,
                ROUND(a.x, int), ROUND(a.y, int),
                ROUND(b.x, int), ROUND(b.y, int),
                ROUND(c.x, int), ROUND(c.y, int),
                border, fill);
}
