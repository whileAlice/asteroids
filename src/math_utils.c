#include "math_utils.h"

#include <assert.h>

float
fractional_part (float n)
{
   const float integer_part = (float)(int)n;

   return n > 0 ? n - integer_part : n + integer_part;
}

bool
is_clockwise (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y)
{
   return (c_x - b_x) * (a_y - b_y) - (c_y - b_y) * (a_x - b_x) < 0;
}

Vector2
center_horizontally (Vector2 origin, int rectangle_width, int container_width)
{
   assert (rectangle_width <= container_width);

   const int new_origin_x = container_width / 2 - rectangle_width / 2;

   return (Vector2){
      .x = (float)(new_origin_x) + origin.x,
      .y = origin.y,
   };
}
