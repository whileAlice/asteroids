#pragma once

#include "context.h"

#include <raylib.h>

// clang-format off
bool game_init   (Context* c);
bool game_deinit (Context* c);
void game_update (Context* c, float dt);
void game_draw   (Context* c);
