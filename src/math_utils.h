#pragma once

#include <stddef.h>
#include <raylib.h>

#define MIN(a, b)          (((a) < (b)) ? (a) : (b))
#define MAX(a, b)          (((a) < (b)) ? (b) : (a))
#define CLAMP(n, min, max) ((min) > (n)) ? (min) : ((max) < (n)) ? (max) : (n)
#define SWAP(x, y)         do { typeof(x) _TEMP = x; x = y; y = _TEMP; } while (0)

float fractional_part(float n);
int index_from_xy(Image *image, size_t x, size_t y);
