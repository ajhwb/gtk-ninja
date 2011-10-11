/**
 * Demonstrate base Glib main event loop functionality, this show
 * what exactly g_timeout_add() and friends did to create a new source
 * but within specific thread.
 *
 * Copyright (C) 2011 - Ardhan Madras <ajhwb@knac.com>
 */

#include <glib.h>

typedef struct 
{
  GMainLoop *loop;
  guint n_run;
} LoopInfo;

gboolean callback (LoopInfo *info)
{
  info->n_run--;;

  g_print ("loop %p n_run: %u\n", info->loop, info->n_run);

  if (info->n_run <= 0)
    {
      g_main_loop_quit (info->loop);
      g_print ("loop %p quit\n", info->loop);
      return FALSE;
    }

  return TRUE;
}

gpointer *thread1 (gpointer *data)
{
  GMainLoop *loop = g_main_loop_new (NULL, FALSE);
  GSource *source = g_timeout_source_new (2250);
  LoopInfo info = { .loop = loop, .n_run = 3 };

  g_source_set_callback (source, (GSourceFunc) callback, &info, NULL);
  g_source_attach (source, NULL);
  g_print ("loop %p run\n", loop);
  g_main_loop_run (loop);

  g_source_unref (source);
  g_main_loop_unref (loop);

  return NULL;
}

gint main(void)
{
  g_thread_init (NULL);
  GThread *thread = g_thread_create ((GThreadFunc) thread1, NULL, TRUE, NULL);

  GMainContext *ctx = g_main_context_new ();
  GMainLoop *loop = g_main_loop_new (ctx, FALSE);
  GSource *source = g_timeout_source_new_seconds (1);
  LoopInfo info = { .loop = loop, .n_run = 5 };

  g_source_set_callback (source, (GSourceFunc) callback, &info, NULL);
  g_source_attach (source, ctx);
  g_print ("loop %p run\n", loop);
  g_main_loop_run (loop);

  g_thread_join (thread);

  g_source_unref (source);
  g_main_loop_unref (loop);

  return 0;
}
