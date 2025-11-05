#pragma once

#include "context.h"

int get_integer_scale();
int get_texture_origin_x();
int get_texture_origin_y();
int get_mouse_x(Context* c);
int get_mouse_y(Context* c);
int buffer_x_from_screen_x(Context* c, int screen_x);
int buffer_y_from_screen_y(Context* c, int screen_y);
int screen_x_from_buffer_x(Context* c, int buf_x);
int screen_y_from_buffer_y(Context* c, int buf_y);
void set_mouse_pos(Context* c, Vector2 pos);
