#include "context.h"

#include "log.h"

#include <stdlib.h>

Context*
context_create (void)
{
   Context* c = malloc (sizeof (Context));
   if (c == NULL)
      ERRNO_RETURN (NULL, "malloc");

   c->event = calloc (1, sizeof (Event));
   if (c->event == NULL)
      ERRNO_RETURN (NULL, "app calloc");

   c->fonts = calloc (1, sizeof (Fonts));
   if (c->fonts == NULL)
      ERRNO_RETURN (NULL, "fonts calloc");

   c->input = calloc (1, sizeof (Input));
   if (c->input == NULL)
      ERRNO_RETURN (NULL, "input calloc");

   c->log = log_create ();
   if (c->log == NULL)
      ERROR_RETURN (NULL, "log create");

   c->pixel_buffer = calloc (1, sizeof (PixelBuffer));
   if (c->pixel_buffer == NULL)
      ERRNO_RETURN (NULL, "pixel buffer calloc");

   c->state = calloc (1, sizeof (State));
   if (c->state == NULL)
      ERRNO_RETURN (NULL, "state calloc");

   return c;
}

void
context_free (Context* c)
{
   if (c == NULL)
      return;

   free (c->event);
   free (c->fonts);
   free (c->input);
   log_free (c->log);
   free (c->pixel_buffer);
   free (c->state);

   free (c);
}
