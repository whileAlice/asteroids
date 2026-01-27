#include "draw_utils.h"

#include "error.h"
#include "font.h"
#include "math_utils.h"
#include "raylib.h"
#include "string.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_PAGE_INDICES 256

static Image*           s_buffer;
static const FixedFont* s_font;

void
set_draw_buffer (Image* buffer)
{
   s_buffer = buffer;
}

void
set_draw_font (const FixedFont* font)
{
   s_font = font;
}

void
draw_rgb_pixel (int x, int y, Color3 pixel)
{
   int index = index_from_xy (s_buffer, x, y);
   if (index == -1)
      return;

   Color3* pixels_buf = (Color3*)s_buffer->data;
   pixels_buf[index]  = pixel;
}

void
draw_rgb_pixel_unsafe (int x, int y, Color3 pixel)
{
   size_t index = index_from_xy_unsafe (s_buffer, x, y);

   Color3* pixels_buf = (Color3*)s_buffer->data;
   pixels_buf[index]  = pixel;
}

void
draw_rgba_pixel (int x, int y, Color pixel)
{
   int index = index_from_xy (s_buffer, x, y);
   if (index == -1)
      return;

   blend_rgba_pixel_on_rgb_buffer (index, pixel);
}

void
draw_rgba_pixel_unsafe (int x, int y, Color pixel)
{
   size_t index = index_from_xy_unsafe (s_buffer, x, y);

   blend_rgba_pixel_on_rgb_buffer (index, pixel);
}

static void
blend_rgba_pixel_on_rgb_buffer (size_t index, Color pixel)
{
   Color3* pixels_buf = (Color3*)s_buffer->data;

   switch (pixel.a)
   {
      case 0: return;
      case 255:
         pixels_buf[index].r = pixel.r;
         pixels_buf[index].g = pixel.g;
         pixels_buf[index].b = pixel.b;
         return;
      default: blend_rgba_pixel_on_rgb_pixel (&pixels_buf[index], &pixel);
   }
}

void
draw_rgb_image_i (int origin_x, int origin_y, const Image* img)
{
   assert (img->format == PIXELFORMAT_UNCOMPRESSED_R8G8B8);

   const int start_x = CLAMP (origin_x, 0, s_buffer->width - 1);
   const int start_y = CLAMP (origin_y, 0, s_buffer->height - 1);
   const int end_x   = CLAMPR (start_x + img->width - 1, s_buffer->width - 1);
   const int end_y   = CLAMPR (start_y + img->height - 1, s_buffer->height - 1);

   Color3* pixels_buf = (Color3*)s_buffer->data;
   Color3* pixels_img = (Color3*)img->data;
   int     img_x, img_y = 0;

   // TODO: this looks like it could be optimized?
   for (int buf_y = start_y; buf_y <= end_y; ++buf_y)
   {
      img_x = 0;

      for (int buf_x = start_x; buf_x <= end_x; ++buf_x)
      {
         size_t index_buf = index_from_xy_unsafe (s_buffer, buf_x, buf_y);
         size_t index_img = index_from_xy_unsafe (img, img_x, img_y);

         pixels_buf[index_buf] = pixels_img[index_img];
         img_x++;
      }

      img_y++;
   }
}

void
draw_rgba_image_i (int origin_x, int origin_y, const Image* img)
{
   assert (img->format == PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

   const int start_x = CLAMP (origin_x, 0, s_buffer->width - 1);
   const int start_y = CLAMP (origin_y, 0, s_buffer->height - 1);
   const int end_x   = CLAMPR (start_x + img->width - 1, s_buffer->width - 1);
   const int end_y   = CLAMPR (start_y + img->height - 1, s_buffer->height - 1);

   Color3* pixels_buf = (Color3*)s_buffer->data;
   Color*  pixels_img = (Color*)img->data;
   int     img_x, img_y = 0;

   // TODO: this looks like it could be optimized?
   for (int buf_y = start_y; buf_y <= end_y; ++buf_y)
   {
      img_x = 0;

      for (int buf_x = start_x; buf_x <= end_x; ++buf_x)
      {
         size_t index_buf = index_from_xy_unsafe (s_buffer, buf_x, buf_y);
         size_t index_img = index_from_xy_unsafe (img, img_x, img_y);

         blend_rgba_pixel_on_rgb_pixel (&pixels_buf[index_buf],
                                        &pixels_img[index_img]);
         img_x++;
      }

      img_y++;
   }
}

void
draw_rgb_image (Vector2 origin, const Image* img)
{
   draw_rgb_image_i ((int)roundf (origin.x), (int)roundf (origin.y), img);
}

void
draw_rgba_image (Vector2 origin, const Image* img)
{
   draw_rgba_image_i ((int)roundf (origin.x), (int)roundf (origin.y), img);
}

void
draw_rgb_overlay (const Image* img)
{
   assert (img->width == s_buffer->width);
   assert (img->height == s_buffer->height);

   Color3* dst_addr = s_buffer->data;
   Color3* src_addr = img->data;
   size_t  count    = img->width * img->height;

   while (count--)
      *dst_addr++ = *src_addr++;
}

void
draw_glyph (int origin_x, int origin_y, size_t glyph_index)
{
   const int total_glyph_width = s_font->glyph_width +
                                 s_font->glyph_margins.left +
                                 s_font->glyph_margins.right;
   const int total_glyph_height = s_font->glyph_height +
                                  s_font->glyph_margins.top +
                                  s_font->glyph_margins.bottom;

   if (origin_x * -1 > s_font->glyph_width)
      return;
   if (origin_y * -1 > s_font->glyph_height)
      return;

   int row          = 0;
   int init_sheet_x = 0 + s_font->glyph_margins.left;
   for (size_t i = 0; i < glyph_index; ++i)
   {
      init_sheet_x += total_glyph_width;

      if (init_sheet_x >= s_font->glyph_sheet->width)
      {
         init_sheet_x -= s_font->glyph_sheet->width;
         row++;
      }
   }

   int sheet_y = row * total_glyph_height + s_font->glyph_margins.top;

   int effective_glyph_width  = s_font->glyph_width;
   int effective_glyph_height = s_font->glyph_height;

   if (origin_x < 0)
   {
      effective_glyph_width += origin_x;
      init_sheet_x          -= origin_x;
   }

   if (origin_y < 0)
   {
      effective_glyph_height += origin_y;
      sheet_y                -= origin_y;
   }

   // TODO: make a function out of this
   const int start_x = CLAMP (origin_x, 0, s_buffer->width - 1);
   const int start_y = CLAMP (origin_y, 0, s_buffer->height - 1);

   const int end_x =
      CLAMPR (start_x + effective_glyph_width - 1, s_buffer->width - 1);
   const int end_y =
      CLAMPR (start_y + effective_glyph_height - 1, s_buffer->height - 1);

   // TODO: this could be generalized
   Color3* pixels_buf   = (Color3*)s_buffer->data;
   Color*  pixels_sheet = (Color*)s_font->glyph_sheet->data;

   // TODO: this looks like it could be optimized?
   for (int buf_y = start_y; buf_y <= end_y; ++buf_y)
   {
      int sheet_x = init_sheet_x;

      for (int buf_x = start_x; buf_x <= end_x; ++buf_x)
      {
         size_t index_buf = index_from_xy_unsafe (s_buffer, buf_x, buf_y);
         size_t index_sheet =
            index_from_xy_unsafe (s_font->glyph_sheet, sheet_x, sheet_y);

         blend_rgba_pixel_on_rgb_pixel (&pixels_buf[index_buf],
                                        &pixels_sheet[index_sheet]);

         sheet_x++;
      }

      sheet_y++;
   }
}

size_t
draw_text_i (int origin_x, int origin_y, int max_width, int max_height,
             const char* text)
{
   assert (text != NULL);
   assert (s_font->glyph_count > 0);

   const size_t text_len = strlen (text);
   const int    stride_x = s_font->glyph_width + s_font->glyph_spacing;
   const int    stride_y = s_font->glyph_height + s_font->glyph_spacing;

   const int max_x = max_width ? CLAMPR (origin_x + max_width, s_buffer->width)
                               : s_buffer->width;
   const int max_y = max_height
                      ? CLAMPR (origin_y + max_height, s_buffer->height)
                      : s_buffer->height;

   int current_origin_x = origin_x;
   int current_origin_y = origin_y;

   for (size_t i = 0; i < text_len; ++i)
   {
      if (max_width == 0 && current_origin_x >= s_buffer->width)
         return i;

      switch (text[i])
      {
         case ' ': current_origin_x += stride_x; continue;
         case '\n':
            current_origin_x  = origin_x;
            current_origin_y += stride_y;
            continue;
      }

      if (max_width > 0 && current_origin_x > max_x - s_font->glyph_width)
      {
         current_origin_x  = origin_x;
         current_origin_y += stride_y;
      }

      if (current_origin_y > max_y)
         return i;

      const int glyph_index = text[i] - 33;

      if (glyph_index < 0)
      {
         current_origin_x += stride_x;
         continue;
      }

      draw_glyph (current_origin_x, current_origin_y, glyph_index);
      current_origin_x += stride_x;
   }

   return 0;
}

// TODO: this is exactly like draw_text_i - can it be combined?
size_t
get_next_text_page_index_i (int origin_x, int origin_y, int max_width,
                            int max_height, const char* text)
{
   assert (text != NULL);

   const size_t text_len = strlen (text);
   const int    stride_x = s_font->glyph_width + s_font->glyph_spacing;
   const int    stride_y = s_font->glyph_height + s_font->glyph_spacing;

   const int max_x = max_width ? CLAMPR (origin_x + max_width, s_buffer->width)
                               : s_buffer->width;
   const int max_y = max_height
                      ? CLAMPR (origin_y + max_height, s_buffer->height)
                      : s_buffer->height;

   int current_origin_x = origin_x;
   int current_origin_y = origin_y;

   for (size_t i = 0; i < text_len; ++i)
   {
      if (max_width == 0 && current_origin_x >= s_buffer->width)
         return i;

      switch (text[i])
      {
         case ' ': current_origin_x += stride_x; continue;
         case '\n':
            current_origin_x  = origin_x;
            current_origin_y += stride_y;
            continue;
      }

      if (max_width > 0 && current_origin_x > max_x - s_font->glyph_width)
      {
         current_origin_x  = origin_x;
         current_origin_y += stride_y;
      }

      if (current_origin_y > max_y)
         return i;

      const int glyph_index = text[i] - 33;

      if (glyph_index < 0)
      {
         current_origin_x += stride_x;
         continue;
      }

      current_origin_x += stride_x;
   }

   return 0;
}

size_t
get_next_text_line_index_i (int origin_x, int max_width, const char* text)
{
   assert (text != NULL);

   const int stride_x = s_font->glyph_width + s_font->glyph_spacing;
   const int max_x = max_width ? CLAMPR (origin_x + max_width, s_buffer->width)
                               : s_buffer->width;

   for (size_t i = 0; i < strlen (text); ++i)
   {
      if (text[i] == '\n')
         return i + 1;

      if (origin_x > max_x - s_font->glyph_width)
         return i;

      origin_x += stride_x;
   }

   return 0;
}

Indices
get_text_page_indices_i (int origin_x, int origin_y, int max_width,
                         int max_height, const char* text)
{
   size_t data_tmp[MAX_TEXT_PAGE_INDICES];

   size_t i, next_index, total_index;
   i = next_index = total_index = 0;
   data_tmp[0]                  = 0;
   while ((next_index =
              get_next_text_page_index_i (origin_x, origin_y, max_width,
                                          max_height, &text[total_index])) > 0)
   {
      total_index   += next_index;
      data_tmp[++i]  = total_index;
   }

   size_t  count     = i + 1;
   size_t  data_size = sizeof (size_t) * count;
   size_t* data      = malloc (data_size);
   memcpy (data, data_tmp, data_size);

   return (Indices){
      .data  = data,
      .count = count,
   };
}

void
copy_text_line_indices_i (Indices* indices, int origin_x, int max_width,
                          const char* text)
{
   size_t i, next_index, total_index;
   i = next_index = total_index = 0;
   while ((next_index = get_next_text_line_index_i (origin_x, max_width,
                                                    &text[total_index])) > 0)
   {
      total_index        += next_index;
      indices->data[++i]  = total_index;
   }

   indices->count = i + 1;
}

void
indices_free (Indices indices)
{
   free (indices.data);
}

void
draw_text (Vector2 origin, int max_width, int max_height, const char* text)
{
   draw_text_i ((int)roundf (origin.x), (int)roundf (origin.y), max_width,
                max_height, text);
}

void
draw_textf_i (int origin_x, int origin_y, int max_width, int max_height,
              const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   char* str = vstrdupf (fmt, args);

   va_end (args);

   draw_text_i (origin_x, origin_y, max_width, max_height, str);
   free (str);
}

void
draw_textf (Vector2 origin, int max_width, int max_height, const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   char* str = vstrdupf (fmt, args);

   va_end (args);

   draw_text (origin, max_width, max_height, str);
   free (str);
}

// TODO: center vertically as well
int
draw_text_center_i (int origin_x, int origin_y, int max_width, int max_height,
                    const char* text)
{
   int text_width = get_text_width (text);

   int origin_x_centered =
      center_horizontally_i (origin_x, text_width, max_width);
   draw_text_i (origin_x_centered, origin_y, max_width * 2, max_height, text);

   return origin_x_centered;
}

Vector2
draw_text_center (Vector2 origin, int max_width, int max_height,
                  const char* text)
{
   int text_width = get_text_width (text);

   Vector2 origin_centered =
      center_horizontally (origin, text_width, max_width);
   draw_text (origin_centered, max_width * 2, max_height, text);

   return origin_centered;
}

int
draw_textf_center_i (int origin_x, int origin_y, int max_width, int max_height,
                     const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   char* str = vstrdupf (fmt, args);

   int new_origin_x =
      draw_text_center_i (origin_x, origin_y, max_width, max_height, str);
   free (str);

   return new_origin_x;
}

Vector2
draw_textf_center (Vector2 origin, int max_width, int max_height,
                   const char* fmt, ...)
{
   va_list args;
   va_start (args, fmt);

   char* str = vstrdupf (fmt, args);

   Vector2 new_origin = draw_text_center (origin, max_width, max_height, str);
   free (str);

   return new_origin;
}

int
get_text_width (const char* text)
{
   size_t text_len = strlen (text);
   if (text_len == 0)
      return 0;

   int width = s_font->glyph_width;

   for (size_t i = 1; i < strlen (text); ++i)
   {
      width += s_font->glyph_width + s_font->glyph_spacing;
   }

   return width;
}

void
draw_circle_fi (int center_x, int center_y, int radius, Color color)
{
   if (radius < 1)
      return;

   const int start_x = CLAMP (center_x - radius, 0, s_buffer->width - 1);
   const int start_y = CLAMP (center_y - radius, 0, s_buffer->height - 1);
   const int end_x   = CLAMPR (start_x + radius * 2 - 1, s_buffer->width - 1);
   const int end_y   = CLAMPR (start_y + radius * 2 - 1, s_buffer->height - 1);

   for (int y = start_y; y <= end_y; ++y)
   {
      for (int x = start_x; x <= end_x; ++x)
      {
         // restoring normal calculation for radius 1 to get 1px circle
         const int radius_mod = radius == 1 ? 0 : radius;
         // TODO: turns out that subtracting radius makes the circle round -
         // why? there could be a nicer method of doing this as well
         if ((center_x - x) * (center_x - x) + (center_y - y) * (center_y - y) <
             radius * radius - radius_mod)
         {
            draw_rgba_pixel_unsafe (x, y, color);
         }
      }
   }
}

void
draw_circle_f (Vector2 center, float radius, Color color)
{
   draw_circle_fi ((int)roundf (center.x), (int)roundf (center.y),
                   (int)roundf (radius), color);
}

void
draw_line_i (int start_x, int start_y, int end_x, int end_y, Color pixel)
{
   bool is_steep = abs (end_y - start_y) > abs (end_x - start_x);

   if (is_steep)
   {
      SWAP (start_x, start_y);
      SWAP (end_x, end_y);
   }

   if (start_x > end_x)
   {
      SWAP (start_x, end_x);
      SWAP (start_y, end_y);
   }

   float dx = (float)end_x - (float)start_x;
   float dy = (float)end_y - (float)start_y;

   float gradient    = dx != 0.0f ? dy / dx : 1.0f;
   float intersect_y = (float)start_y;

   if (is_steep)
   {
      for (int i = start_x; i <= end_x; ++i)
      {
         pixel.a = (uint8_t)(255.f * fractional_part (intersect_y));
         draw_rgba_pixel ((int)(intersect_y) + 1, i, pixel);

         pixel.a = (uint8_t)(255.f * (1.0f - fractional_part (intersect_y)));
         draw_rgba_pixel ((int)(intersect_y), i, pixel);

         intersect_y += gradient;
      }
   }
   else
   {
      for (int i = start_x; i <= end_x; ++i)
      {
         pixel.a = (uint8_t)(255.f * fractional_part (intersect_y));
         draw_rgba_pixel (i, (int)(intersect_y) + 1, pixel);

         pixel.a = (uint8_t)(255.f * (1.0f - fractional_part (intersect_y)));
         draw_rgba_pixel (i, (int)(intersect_y), pixel);

         intersect_y += gradient;
      }
   }
}

void
draw_line (Vector2 start, Vector2 end, Color pixel)
{
   draw_line_i ((int)roundf (start.x), (int)roundf (start.y),
                (int)roundf (end.x), (int)roundf (end.y), pixel);
}

void
draw_rectangle_fi (int origin_x, int origin_y, int width, int height,
                   Color pixel)
{
   if (width == 0 || height == 0)
      return;

   const int start_x = CLAMP (origin_x, 0, s_buffer->width - 1);
   const int start_y = CLAMP (origin_y, 0, s_buffer->height - 1);
   const int end_x   = CLAMPR (start_x + width - 1, s_buffer->width - 1);
   const int end_y   = CLAMPR (start_y + height - 1, s_buffer->height - 1);

   for (int y = start_y; y <= end_y; ++y)
   {
      for (int x = start_x; x <= end_x; ++x)
      {
         draw_rgba_pixel_unsafe (x, y, pixel);
      }
   }
}

void
draw_rectangle_f (Vector2 origin, Vector2 size, Color pixel)
{
   draw_rectangle_fi ((int)roundf (origin.x), (int)roundf (origin.y),
                      (int)roundf (size.x), (int)roundf (size.y), pixel);
}

void
draw_rectangle_wi (int origin_x, int origin_y, int width, int height,
                   Color pixel)
{
   draw_quad_wi (origin_x, origin_y, origin_x + width, origin_y,
                 origin_x + width, origin_y + height, origin_x,
                 origin_y + height, pixel);
}

void
draw_rectangle_i (int origin_x, int origin_y, int width, int height,
                  Color border, Color fill)
{
   draw_rectangle_fi (origin_x, origin_y, width, height, fill);
   draw_rectangle_wi (origin_x, origin_y, width, height, border);
}

void
draw_rectangle (Vector2 origin, Vector2 size, Color border, Color fill)
{
   draw_rectangle_i ((int)roundf (origin.x), (int)roundf (origin.y),
                     (int)roundf (size.x), (int)roundf (size.y), border, fill);
}

void
draw_triangle_fi (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y,
                  Color pixel)
{
   // TODO: convert to scanline-based approach
   if (!is_clockwise (a_x, a_y, b_x, b_y, c_x, c_y))
   {
      SWAP (a_x, b_x);
      SWAP (a_y, b_y);
   }

   const int min_x = CLAMP (MIN (MIN (a_x, b_x), c_x), 0, s_buffer->width - 1);
   const int min_y = CLAMP (MIN (MIN (a_y, b_y), c_y), 0, s_buffer->height - 1);
   const int max_x = CLAMP (MAX (MAX (a_x, b_x), c_x), 0, s_buffer->width - 1);
   const int max_y = CLAMP (MAX (MAX (a_y, b_y), c_y), 0, s_buffer->height - 1);

   for (int y = min_y; y <= max_y; ++y)
   {
      for (int x = min_x; x <= max_x; ++x)
      {
         if (is_clockwise (a_x, a_y, b_x, b_y, x, y) &&
             is_clockwise (b_x, b_y, c_x, c_y, x, y) &&
             is_clockwise (c_x, c_y, a_x, a_y, x, y))
         {
            draw_rgba_pixel_unsafe (x, y, pixel);
         }
      }
   }
}

void
draw_triangle_wi (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y,
                  Color pixel)
{
   draw_line_i (a_x, a_y, b_x, b_y, pixel);
   draw_line_i (b_x, b_y, c_x, c_y, pixel);
   draw_line_i (c_x, c_y, a_x, a_y, pixel);
}

void
draw_triangle_i (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y,
                 Color border, Color fill)
{
   draw_triangle_fi (a_x, a_y, b_x, b_y, c_x, c_y, fill);
   draw_triangle_wi (a_x, a_y, b_x, b_y, c_x, c_y, border);
}

void
draw_triangle (Vector2 a, Vector2 b, Vector2 c, Color border, Color fill)
{
   draw_triangle_i ((int)roundf (a.x), (int)roundf (a.y), (int)roundf (b.x),
                    (int)roundf (b.y), (int)roundf (c.x), (int)roundf (c.y),
                    border, fill);
}

void
draw_quad_wi (int a_x, int a_y, int b_x, int b_y, int c_x, int c_y, int d_x,
              int d_y, Color pixel)
{
   draw_line_i (a_x, a_y, b_x, b_y, pixel);
   draw_line_i (b_x, b_y, c_x, c_y, pixel);
   draw_line_i (c_x, c_y, d_x, d_y, pixel);
   draw_line_i (d_x, d_y, a_x, a_y, pixel);
}

void
box_blur (Image* dst, const Image* src, size_t iterations)
{
   assert (iterations >= 1);

   Color3* pixels  = src->data;
   Color3* blurred = dst->data;
   Color3* tmp     = NULL;

   size_t size = src->width * src->height * sizeof (Color3);
   size_t i    = iterations;
   while (i--)
   {
      for (int y = 0; y < src->height; ++y)
      {
         for (int x = 0; x < src->width; ++x)
         {
            if (x < 1 || y < 1 || x + 1 == src->width || y + 1 == src->height)
            {
               // 9x9 kernel does not fit, leave pixel as is
               const size_t index = index_from_xy_unsafe (src, x, y);
               blurred[index]     = pixels[index];

               continue;
            }

            int sum_r = 0;
            int sum_g = 0;
            int sum_b = 0;
            // take average from 8 neighboring pixels and the current pixel
            // NOTE: a bit slow, but I think it should be left to the compiler
            // or maybe think about how the window moves and how many pixels
            // are the same vs previous target pixel
            for (int n_y = -1; n_y <= 1; ++n_y)
            {
               for (int n_x = -1; n_x <= 1; ++n_x)
               {
                  const size_t neighbor_index =
                     index_from_xy_unsafe (src, x + n_x, y + n_y);
                  sum_r += pixels[neighbor_index].r;
                  sum_g += pixels[neighbor_index].g;
                  sum_b += pixels[neighbor_index].b;
               }
            }

            const size_t index = index_from_xy_unsafe (src, x, y);
            blurred[index].r   = (uint8_t)(sum_r / 9);
            blurred[index].g   = (uint8_t)(sum_g / 9);
            blurred[index].b   = (uint8_t)(sum_b / 9);
         }
      }

      if (i > 0)
      {
         if (iterations - i == 1)
         {
            tmp = malloc (size);
         }

         memcpy (tmp, blurred, size);
         pixels = tmp;
      }
   }

   if (tmp != NULL)
   {
      free (tmp);
   }
}

void
brighten_image_by_amount (Image* dst, const Image* src, int amount)
{
   assert (dst->width == src->width && dst->height == src->height);
   assert (dst->format == src->format);

   size_t size = src->width * src->height;
   if (src->format == PIXELFORMAT_UNCOMPRESSED_R8G8B8)
   {
      const Color3* pixels = src->data;
      Color3*       result = dst->data;

      for (size_t i = 0; i < size; ++i)
      {
         result[i].r = (uint8_t)CLAMP ((int)pixels[i].r + amount, 0, 255);
         result[i].g = (uint8_t)CLAMP ((int)pixels[i].g + amount, 0, 255);
         result[i].b = (uint8_t)CLAMP ((int)pixels[i].b + amount, 0, 255);
      }
   }
   else
   {
      const Color* pixels = src->data;
      Color*       result = dst->data;

      for (size_t i = 0; i < size; ++i)
      {
         result[i].r = (uint8_t)CLAMP ((int)pixels[i].r + amount, 0, 255);
         result[i].g = (uint8_t)CLAMP ((int)pixels[i].g + amount, 0, 255);
         result[i].b = (uint8_t)CLAMP ((int)pixels[i].b + amount, 0, 255);
      }
   }
}

// TODO: both brightens share almost everything, it can be reused
void
brighten_image_by_percentage (Image* dst, const Image* src, int percentage)
{
   assert (dst->width == src->width && dst->height == src->height);
   assert (dst->format == src->format);

   size_t size = src->width * src->height;
   if (src->format == PIXELFORMAT_UNCOMPRESSED_R8G8B8)
   {
      const Color3* pixels = src->data;
      Color3*       result = dst->data;

      for (size_t i = 0; i < size; ++i)
      {
         result[i].r =
            (uint8_t)CLAMP ((int)pixels[i].r * percentage / 100, 0, 255);
         result[i].g =
            (uint8_t)CLAMP ((int)pixels[i].g * percentage / 100, 0, 255);
         result[i].b =
            (uint8_t)CLAMP ((int)pixels[i].b * percentage / 100, 0, 255);
      }
   }
   else
   {
      const Color* pixels = src->data;
      Color*       result = dst->data;

      for (size_t i = 0; i < size; ++i)
      {
         result[i].r =
            (uint8_t)CLAMP ((int)pixels[i].r * percentage / 100, 0, 255);
         result[i].g =
            (uint8_t)CLAMP ((int)pixels[i].g * percentage / 100, 0, 255);
         result[i].b =
            (uint8_t)CLAMP ((int)pixels[i].b * percentage / 100, 0, 255);
      }
   }
}

void
clear_buffer (Color3 pixel)
{
   clear_rgb_image (s_buffer, pixel);
}

void
clear_rgb_image (Image* img, Color3 pixel)
{
   Color3* addr  = img->data;
   size_t  count = img->width * img->height;

   while (count--)
      *addr++ = pixel;
}

Image*
clone_image (const Image* src)
{
   Image* image = malloc (sizeof (Image));
   if (image == NULL)
      ERRNO_RETURN (NULL, "malloc");

   size_t type_size = src->format == PIXELFORMAT_UNCOMPRESSED_R8G8B8
                       ? sizeof (Color3)
                       : sizeof (Color);
   size_t size      = src->width * src->height * type_size;
   void*  pixels    = (void*)malloc (size);

   memcpy (pixels, src->data, size);

   *image = (Image){
      .data    = pixels,
      .width   = src->width,
      .height  = src->height,
      .mipmaps = src->mipmaps,
      .format  = src->format,
   };

   return image;
}

void
image_free (Image* image)
{
   free (image->data);
   free (image);
}

int
index_from_xy (const Image* img, int x, int y)
{
   const int index     = img->width * y + x;
   const int max_index = img->width * img->height - 1;
   assert (max_index >= 0);

   return index >= 0 && index <= max_index ? index : -1;
}

size_t
index_from_xy_unsafe (const Image* img, int x, int y)
{
   return img->width * y + x;
}

Color3
rgb_from_rgba (Color pixel)
{
   typedef union {
      Color  rgba;
      Color3 rgb;
   } converter;

   converter c;
   c.rgba = pixel;

   return c.rgb;
}

Color
rgba_from_rgb (Color3 pixel)
{
   return (Color){
      .r = pixel.r,
      .g = pixel.g,
      .b = pixel.b,
      .a = 255,
   };
}

void
blend_rgba_pixel_on_rgb_pixel (Color3* dst, Color* src)
{
   dst->r = (dst->r * (255 - src->a) + src->r * src->a + 128) / 255;
   dst->g = (dst->g * (255 - src->a) + src->g * src->a + 128) / 255;
   dst->b = (dst->b * (255 - src->a) + src->b * src->a + 128) / 255;
}
