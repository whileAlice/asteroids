#pragma once

#include "font.h"

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct color_3 {
   uint8_t r;
   uint8_t g;
   uint8_t b;
} Color3;

// clang-format off

/* sets target draw buffer */
void set_draw_buffer (Image* buffer);

/* sets the font used for drawing text */
void set_draw_font (const FixedFont* font);

/* draws a non-alpha-blended pixel */
void draw_rgb_pixel (int x, int y, Color3 pixel);

/* draws a non-alpha-blended pixel without bounds checking */
void draw_rgb_pixel_unsafe (int x, int y, Color3 pixel);

/* draws an alpha-blended-pixel */
void draw_rgba_pixel (int x, int y, Color pixel);

/* draws an alpha-blended-pixel without bounds checking */
void draw_rgba_pixel_unsafe (int x, int y, Color pixel);

/* blends an rgba pixel onto a rgb buffer */
static void blend_rgba_pixel_on_rgb_buffer (size_t index, Color pixel);

/* draws a non-alpha-blended image (int) */
void draw_rgb_image_i (int origin_x, int origin_y, const Image* img);

/* draws an alpha-blended image (int) */
void draw_rgba_image_i (int origin_x, int origin_y, const Image* img);

/* draws a non-alpha-blended image */
void draw_rgb_image (Vector2 origin, const Image* img);

/* draws an alpha-blended image */
void draw_rgba_image (Vector2 origin, const Image* img);

/* draws a glyph using the statically set font */
void draw_glyph (int origin_x, int origin_y, size_t index);

/* draws text using the statically set font (int) */
void draw_text_i (int origin_x, int origin_y, const char* text);

/* draws text using the statically set font */
void draw_text (Vector2 origin, const char* text);

/* draws formatted text using the statically set font (int) */
void draw_textf_i (int origin_x, int origin_y, const char* fmt, ...);

/* draws formatted text using the statically set font */
void draw_textf (Vector2 origin, const char* fmt, ...);

/* draws centered text using the statically set font; returns new origin x (int) */
int draw_text_center_i (int origin_x, int origin_y, int max_width, const char* text);

/* draws centered text using the statically set font; returns new origin */
Vector2 draw_text_center (Vector2 origin, int max_width, const char* text);

/* draws formatted, centered text using the statically set font; returns new origin x (int) */
int draw_textf_center_i (int origin_x, int origin_y, int max_width, const char* fmt, ...);

/* draws formatted, centered text using the statically set font; returns new origin */
Vector2 draw_textf_center (Vector2 origin, int max_width, const char* fmt, ...);

/* gets text width */
int get_text_width (const char* text);

/* draws a filled circle (int) */
void draw_circle_fi (int center_x, int center_y, int radius, Color color);

/* draws a filled circle */
void draw_circle_f (Vector2 center, float radius, Color color);

/* draws an anti-aliased line (int) */
void draw_line_i (int start_x, int start_y, int end_x, int end_y, Color pixel);

/* draws an anti-aliased line */
void draw_line (Vector2 start, Vector2 end, Color pixel);

/* draws a filled rectangle (int) */
void draw_rectangle_fi (int origin_x, int origin_y, int width, int height, Color pixel);

/* draws a filled rectangle */
void draw_rectangle_f (Vector2 origin, Vector2 size, Color pixel);

/* draws a wireframe rectangle (int) */
void draw_rectangle_wi (int origin_x, int origin_y, int width, int height, Color pixel);

/* draws a filled rectangle with border (int) */
void draw_rectangle_i (int origin_x, int origin_y, int width, int height, Color border, Color fill);

/* draws a filled rectangle with border */
void draw_rectangle (Vector2 origin, Vector2 size, Color border, Color fill);

/* draws a filled non-anti-aliased triangle (int) */
void draw_triangle_fi (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y, Color pixel);

/* draws a wireframe triangle (int) */
void draw_triangle_wi (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y, Color pixel);

/* draws a filled triangle with anti-aliased border (int) */
void draw_triangle_i (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y, Color border, Color fill);

/* draws a filled triangle with anti-aliased border */
void draw_triangle (Vector2 a, Vector2 b, Vector2 c, Color border, Color fill);

/* draws a wireframe quad (int) */
void draw_quad_wi (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y, int d_x, int d_y, Color pixel);

/* blurs an image using box blur */
void box_blur (Image* dst, const Image* src, size_t iterations);

/* brightens or darkens an image by specified amount (0-255) */
void brighten_image_by_amount (Image* dst, const Image* src, int amount);

/* brightens or darkens an image by relative percentage */
void brighten_image_by_percentage (Image* dst, const Image* src, int percentage);

/* clears the current buffer with supplied RGB pixel */
void clear_buffer (Color3 pixel);

/* clears the image with supplied RGB pixel */
void clear_rgb_image (Image* img, Color3 pixel);

/* makes a clone of the given image */
Image* clone_image (const Image* src);

/* calculates index from xy coordinates */
int index_from_xy (const Image* img, int x, int y);

/* calculates index from xy coordinates without bounds checking */
size_t index_from_xy_unsafe (const Image* img, int x, int y);

/* converts rgba to rgb */
Color3 rgb_from_rgba (Color pixel);

/* converts rgb to rgba */
Color rgba_from_rgb (Color3 pixel);

/* alpha blends bg with fg */
void blend_rgba_pixel_on_rgb_pixel (Color3* bg, Color* fg);
