#include <assert.h>

#include "math_utils.h"

float
fractional_part(float n)
{
  const float integer_part = (float)(int)n;

  return n > 0 ? n - integer_part : n + integer_part;
}

bool
is_clockwise(int a_x, int a_y, int b_x,
             int b_y, int c_x, int c_y)
{
  return (c_x - b_x) * (a_y - b_y) - (c_y - b_y) * (a_x - b_x) < 0;
}
