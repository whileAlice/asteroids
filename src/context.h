#pragma once

#include "font.h"
#include "log_buffer.h"
#include "scene.h"

#include <pthread.h>
#include <raylib.h>
#include <stddef.h>

#define READ_END  0
#define WRITE_END 1

typedef struct event {
   pthread_mutex_t mutex;
   pthread_cond_t  cond;
   bool            should_quit_app;
   bool            should_deinit_game;
   bool            should_open_main_menu;
   bool            should_close_main_menu;
   bool            should_change_scene;
   SceneID         new_scene;
   bool            has_event;
} Event;

typedef struct fonts {
   FixedFont fixed_font;
   FixedFont fixed_font_inverted;
} Fonts;

typedef struct input {
   bool toggle_osd;
   bool toggle_log;
   bool toggle_main_menu;
   bool log_page_up;
   bool log_page_down;
   bool previous_log;
   bool next_log;
   bool switch_fullscreen;
} Input;

typedef struct pixel_buffer {
   Image*    image;
   Texture2D texture;
   int       texture_origin_x;
   int       texture_origin_y;
   int       integer_scale;
} PixelBuffer;

typedef struct state {
   bool   should_show_osd;
   bool   should_show_log;
   bool   has_overlay;
   bool   is_paused;
   size_t current_log_page;
} State;

typedef struct context {
   Event*       event;
   Fonts*       fonts;
   Input*       input;
   PixelBuffer* pixel_buffer;
   State*       state;
} Context;

// clang-format off
Context* context_create (void);
void context_free (Context* c);
