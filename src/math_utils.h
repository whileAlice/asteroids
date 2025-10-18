#pragma once

#include <stddef.h>
#include <raylib.h>

#define MIN(a,b)         (((a) < (b)) ? (a) : (b))
#define MAX(a,b)         (((a) < (b)) ? (b) : (a))
#define CLAMP(n,min,max) ((min) > (n)) ? (min) : ((max) < (n)) ? (max) : (n)

void swap(size_t *a, size_t *b);
float fractional_part(float n);
int index_from_xy(Image *image, size_t x, size_t y);
