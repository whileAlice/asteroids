#pragma once

#include <stddef.h>
#include <raylib.h>

#define MIN(a, b)          (((a) < (b)) ? (a) : (b))
#define MAX(a, b)          (((a) < (b)) ? (b) : (a))
#define CLAMP(n, min, max) ((min) > (n)) ? (min) : ((max) < (n)) ? (max) : (n)
#define SWAP(x, y)         do { typeof(x) _TEMP = x; x = y; y = _TEMP; } while (0)

/* returns the fractional part of a given floating-point number */
float fractional_part(float n);
