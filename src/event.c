#include "event.h"

#include "context.h"
#include "error.h"
#include "scene_manager.h"
#include "threads.h"
#include "ui_layer_manager.h"

// TODO: this can be simplified
bool
event_handle (Context* c)
{
   if (should_quit_app (c))
      c->event->should_deinit_game = true;

   if (c->event->should_change_scene)
   {
      if (!set_current_scene (c, c->event->new_scene))
         ERROR_RETURN (false, "set current scene");

      c->event->should_change_scene = false;
   }

   // TODO: isn't this split a bit too much?
   if (c->event->should_open_main_menu)
   {
      if (!add_modal (c, MAIN_MENU_MODAL))
         ERROR_RETURN (false, "add modal");

      c->event->should_open_main_menu = false;
   }

   if (c->event->should_close_main_menu)
   {
      if (!remove_modal (c, MAIN_MENU_MODAL))
         ERROR_RETURN (false, "remove modal");

      c->event->should_close_main_menu = false;
   }

   c->event->has_event = false;

   return true;
}

void
change_scene (Context* c, SceneID new_scene)
{
   c->event->new_scene           = new_scene;
   c->event->should_change_scene = true;

   c->event->has_event = true;
}

void
app_quit_initiate (Context* c)
{
   IN_LOCK(&c->event->mutex,
      c->event->has_event = true;
      c->event->should_quit_app = true;
      pthread_cond_broadcast (&c->event->cond);
   );
}

bool
should_quit_app (Context* c)
{
   bool should_quit = false;

   IN_LOCK(&c->event->mutex,
      should_quit = c->event->should_quit_app;
   );

   return should_quit;
}

void
toggle_main_menu (Context* c)
{
   if (is_modal_open (MAIN_MENU_MODAL))
   {
      c->event->should_close_main_menu = true;
      c->state->is_paused              = false;
   }
   else
   {
      c->event->should_open_main_menu = true;
      c->state->is_paused             = true;
   }

   c->event->has_event = true;
}
