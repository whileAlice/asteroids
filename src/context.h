#pragma once

#include "font.h"
#include "log_buffer.h"

#include <pthread.h>
#include <raylib.h>
#include <stddef.h>

#define READ_END  0
#define WRITE_END 1

typedef struct pixel_buffer {
   Image     image;
   Texture2D texture;
   int       texture_origin_x;
   int       texture_origin_y;
   int       integer_scale;
} PixelBuffer;

typedef struct fonts {
   const FixedFont fixed_font;
   const FixedFont inverted_fixed_font;
} Fonts;

typedef struct input {
   bool show_osd;
   bool show_log;
   bool log_page_up;
   bool log_page_down;
   bool switch_fullscreen;
} Input;

typedef struct log {
   LogBuffer*      buffer;
   pthread_mutex_t mutex;
   pthread_cond_t  cond;
   size_t          thread_ready_count;
   bool            should_abort_init;
   int             stdout_pipe[2];
   int             stderr_pipe[2];
   int             wakeup_pipe[2];
} Log;

typedef struct state {
   pthread_mutex_t mutex;
   pthread_cond_t  cond;
   bool            should_show_osd;
   bool            should_show_log;
   bool            should_exit_app;
   size_t          current_log_page;
} State;

typedef struct context {
   PixelBuffer* pixel_buffer;
   Fonts*       fonts;
   Input*       input;
   Log*         log;
   State*       state;
} Context;

Context* context_create (void);
void     context_deinit (Context* c);
