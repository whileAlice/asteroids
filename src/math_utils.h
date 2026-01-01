#pragma once

#include <raylib.h>

#define MIN(a, b)          (((a) < (b)) ? (a) : (b))
#define MAX(a, b)          (((a) < (b)) ? (b) : (a))
#define CLAMP(n, min, max) (((min) > (n)) ? (min) : ((max) < (n)) ? (max) : (n))
#define CLAMPR(n, max)     (((max) < (n)) ? (max) : (n))
#define SWAP(x, y)              \
   do                           \
   {                            \
      typeof (x) _TEMP = x;     \
      x                = y;     \
      y                = _TEMP; \
   }                            \
   while (0)

/* returns the fractional part of a given floating-point number */
float fractional_part (float n);

/* checks if a given triangle is clockwise */
bool is_clockwise (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y);

/* returns origin of a horizontally centered line; origin.x is offset */
Vector2 center_horizontally (Vector2 origin, int width, int container_width);
