#include "context.h"

#include "log.h"

#include <stdlib.h>

Context*
context_create (void)
{
   Context* c = malloc (sizeof (Context));
   if (c == NULL)
      ERROR_RETURN (NULL, "malloc");

   c->app = calloc (1, sizeof (App));
   if (c->app == NULL)
      ERROR_RETURN (NULL, "app calloc");

   c->fonts = calloc (1, sizeof (Fonts));
   if (c->fonts == NULL)
      ERROR_RETURN (NULL, "fonts calloc");

   c->input = calloc (1, sizeof (Input));
   if (c->input == NULL)
      ERROR_RETURN (NULL, "input calloc");

   c->log = log_create ();
   if (c->log == NULL)
      ERROR_RETURN (NULL, "log create");

   c->pixel_buffer = calloc (1, sizeof (PixelBuffer));
   if (c->pixel_buffer == NULL)
      ERROR_RETURN (NULL, "pixel buffer calloc");

   c->state = calloc (1, sizeof (State));
   if (c->state == NULL)
      ERROR_RETURN (NULL, "state calloc");

   return c;
}

void
context_free (Context* c)
{
   if (c == NULL)
      return;

   free (c->app);
   free (c->fonts);
   free (c->input);
   log_free (c->log);
   free (c->pixel_buffer);
   free (c->state);

   free (c);
}
