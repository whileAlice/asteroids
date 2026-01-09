#pragma once

#include <stdio.h>

typedef struct kevent64_s  Kevent64_s;
typedef struct epoll_event EpollEvent;

typedef enum event_idx {
   STDOUT_EVENT = 0,
   STDERR_EVENT,
   WAKEUP_EVENT,
   EVENT_COUNT,
} EventIdx;

typedef struct stream_context {
   const char* name;
   int         pipe[2];
   int         stream_fd_copy;
   int         stream_fd_original;
   FILE*       stream_file_copy;
   FILE*       stream_file_original;
} StreamContext;

void*      std_streamer_thread           (void* arg);
