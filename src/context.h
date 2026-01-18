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
   bool            should_change_scene;
   SceneID         new_scene;
   bool            has_event;
} Event;

typedef struct fonts {
   FixedFont fixed_font;
   FixedFont inverted_fixed_font;
} Fonts;

typedef struct input {
   bool show_osd;
   bool toggle_log;
   bool log_page_up;
   bool log_page_down;
   bool switch_fullscreen;
} Input;

typedef struct log {
   LogBuffer**     buffers;
   pthread_mutex_t mutex;
   pthread_cond_t  cond;
   size_t          thread_ready_count;
   bool            should_abort_init;
   int             wakeup_pipe[2];
} Log;

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
   size_t current_log_page;
} State;

typedef struct context {
   Event*       event;
   Fonts*       fonts;
   Input*       input;
   Log*         log;
   PixelBuffer* pixel_buffer;
   State*       state;
} Context;

Context* context_create (void);
void     context_free (Context* c);
