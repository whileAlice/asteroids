#pragma once

#include <raylib.h>

#include "context.h"

void game_init(Context* c);
void game_update(Context* c, float dt);
void game_draw(Context* c, Image* buf);
void game_deinit();
