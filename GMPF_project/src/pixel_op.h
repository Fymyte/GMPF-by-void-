#ifndef PIXEL_OP_H_
#define PIXEL_OP_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include <gtk-3.0/gtk/gtk.h>
#include <stdio.h>

void put_pixel(GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha);

gboolean gdkpixbuf_get_colors_by_coordinates(GdkPixbuf *pixbuf, gint x, gint y
        , guchar *red, guchar *green, guchar *blue, guchar *alpha);

#endif
