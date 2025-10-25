#pragma once

#include <stdio.h>

#include "raylib.h"

Image image_from_pam(const char* filename);
char* read_next_line(FILE* fp, char* buf, const char* filename);
void read_all_tokens(FILE* fp, char* buf, const char* filename);
