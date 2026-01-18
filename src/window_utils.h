#pragma once

#include "context.h"

// clang-format off
int  get_integer_scale                   (void);
int  get_texture_origin_x                (void);
int  get_texture_origin_y                (void);
void set_buffer_scale_and_texture_origin (Context* c);
int  get_mouse_x                         (Context* c);
int  get_mouse_y                         (Context* c);
int  buffer_x_from_screen_x              (Context* c, int screen_x);
int  buffer_y_from_screen_y              (Context* c, int screen_y);
int  screen_x_from_buffer_x              (Context* c, int buf_x);
int  screen_y_from_buffer_y              (Context* c, int buf_y);
void set_mouse_pos                       (Context* c, Vector2 pos);
bool should_close_window                 (Context* c);
