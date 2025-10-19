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
