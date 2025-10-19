#pragma once

#include <stdio.h>

#include "raylib.h"

Image image_from_pam(const char *filename);
char *read_next_line(FILE *fp, char *buffer, const char *filename, const char *name);
void seek_to_space(FILE *fp);
int get_next_value(FILE *fp, char *buffer, const char *filename, const char *name);
