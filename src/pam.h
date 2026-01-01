#pragma once

#include "raylib.h"

#include <stdio.h>

Image image_from_pam (const char* filename);
void  read_all_pam_tokens (FILE* fp, char* buf, const char* filename);
