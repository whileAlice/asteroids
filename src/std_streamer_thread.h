#pragma once

#include <stdio.h>

typedef enum event_idx {
   STDOUT_EVENT,
   STDERR_EVENT,
   WAKEUP_EVENT,
   EVENT_COUNT,
} EventIdx;

typedef struct stream_context {
   const char* name;
   int         read_fd;
   int         write_fd;
   FILE*       write_file;
} StreamContext;

void* std_streamer_thread (void* arg);
