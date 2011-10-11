/*
 * Glob usage
 *
 * Copyright (C) 2011 - Ardhan Madras <ajhwb@knac.com>
 */

#include <glib.h>
#include <string.h>

gint main(void)
{
  const gchar ptrn_str[] = "*.playboy.com";
  const gchar str[] = "www.ac2.playboy.com";

  GPatternSpec *ptrn = g_pattern_spec_new (ptrn_str);
  gboolean match = g_pattern_match (ptrn, strlen(str), str, NULL);

  g_print ("pattern %s match\n", match ? "is" : "is not");
  return 0;
}
