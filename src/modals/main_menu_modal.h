#pragma once

#include "../context.h"

#include <raylib.h>

// clang-format off
bool main_menu_modal_init   (Context* c);
bool main_menu_modal_deinit (Context* c);
void main_menu_modal_update (Context* c, float dt);
void main_menu_modal_draw   (Context* c);
void select_option          (Context* c);
void next_option            (void);
void prev_option            (void);
