/*
 * Using Cairo to draw alpha pixbuf as background image, the image
 * is redraw and centered when 'expore-event' signal is occured.
 *
 * Copyright (C) 2011 - Ardhan Madras <ajhwb@knac.com>
 */

#include <gtk/gtk.h>

gboolean
draw (GtkWidget      *widget,
      GdkEventExpose *event,
      gpointer        data)
{
  cairo_t *cr;
  GdkPixbuf *pixbuf = data;
  gint x, y;

  cr = gdk_cairo_create (widget->window);

  gdk_drawable_get_size (widget->window, &x, &y);
  gdk_cairo_set_source_pixbuf (cr, pixbuf, 
                               x / 2 - (gdk_pixbuf_get_width (pixbuf) / 2), 
                               y / 2 - (gdk_pixbuf_get_height (pixbuf) / 2));
  cairo_rectangle (cr, 0, 0,
                   gdk_pixbuf_get_width (pixbuf),
                   gdk_pixbuf_get_height (pixbuf));
  cairo_paint_with_alpha (cr, 0.10);

  cairo_destroy (cr);

  return FALSE;
}

gint
main (gint argc,
      gchar *argv[])
{
  GtkWidget *window;
  GtkWidget *drawing_area;
  GdkPixbuf *pixbuf;

  gtk_init (&argc, &argv);

  pixbuf = gdk_pixbuf_new_from_file ("bg.png", NULL);
  g_return_val_if_fail (pixbuf != NULL, FALSE);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  drawing_area = gtk_drawing_area_new ();
  g_signal_connect (drawing_area, "expose-event", G_CALLBACK (draw), pixbuf);
  gtk_container_add (GTK_CONTAINER (window), drawing_area);

  gtk_widget_show_all (window);

  gtk_main ();

  g_object_unref (pixbuf);

  return 0;
}
