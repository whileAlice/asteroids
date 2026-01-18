#pragma once

#include "raylib.h"

#include <stdio.h>

// clang-format off
Image* image_from_pam      (const char* filename);
bool   read_all_pam_tokens (FILE* fp, char* buf, const char* filename);
