#include <assert.h>

#include "math_utils.h"

float
fractional_part(float n)
{
  const float integer_part = (float)(int)n;

  return n > 0 ? n - integer_part : n + integer_part;
}

int
index_from_xy(Image *image, size_t x, size_t y)
{
  const int index = image->width * y + x;
  const int max_index = image->width * image->height - 1;

	return index >= 0 && index <= max_index ? index : -1;
}
