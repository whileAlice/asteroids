#include <assert.h>

#include "math_utils.h"

float
fractional_part(float n)
{
  const float integer_part = (float)(int)n;

  return n > 0 ? n - integer_part : n + integer_part;
}

