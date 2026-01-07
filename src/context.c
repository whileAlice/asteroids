#include "context.h"

#include "log.h"

#include <stdlib.h>

Context*
context_create (void)
{
   Context* c = malloc (sizeof (Context));
   if (c == NULL)
   {
      perror ("malloc");
      return NULL;
   }

   c->app = calloc (1, sizeof (App));
   if (c->app == NULL)
   {
      perror ("app calloc");
      return NULL;
   }
   c->fonts = calloc (1, sizeof (Fonts));
   if (c->fonts == NULL)
   {
      perror ("fonts calloc");
      return NULL;
   }
   c->input = calloc (1, sizeof (Input));
   if (c->input == NULL)
   {
      perror ("input calloc");
      return NULL;
   }
   c->log = log_create ();
   if (c->log == NULL)
   {
      perror ("log create");
      return NULL;
   }
   c->pixel_buffer = calloc (1, sizeof (PixelBuffer));
   if (c->pixel_buffer == NULL)
   {
      perror ("buffer calloc");
      return NULL;
   }
   c->state = calloc (1, sizeof (State));
   if (c->state == NULL)
   {
      perror ("state calloc");
      return NULL;
   }

   return c;
}

void
context_deinit (Context* c)
{
   if (c == NULL)
      return;

   free (c->pixel_buffer);
   free (c->fonts);
   free (c->input);
   log_deinit (c->log);
   free (c->state);

   free (c);
}
