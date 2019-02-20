#include "pixel_op.h"

void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
{
    int width, height, rowstride, n_channels;
    guchar *pixels, *p;

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);

    g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    g_assert (n_channels == 4);

    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);

    g_assert (x >= 0 && x < width);
    g_assert (y >= 0 && y < height);

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * n_channels;
    p[0] = red;
    p[1] = green;
    p[2] = blue;
    p[3] = alpha;
}

gboolean gdkpixbuf_get_colors_by_coordinates(GdkPixbuf *pixbuf, gint x, gint y
    , guchar *red, guchar *green, guchar *blue, guchar *alpha)
{
    guchar *pixel=NULL;
    gint channel=0;
    gint width=0;

    if (pixbuf == NULL) return FALSE;
    if (x<0 || y<0) return FALSE;
    if (x>=gdk_pixbuf_get_width(pixbuf)) return FALSE;
    if (y>=gdk_pixbuf_get_height(pixbuf)) return FALSE;

    pixel=gdk_pixbuf_get_pixels(pixbuf);
    channel=gdk_pixbuf_get_n_channels(pixbuf);
    width=gdk_pixbuf_get_width(pixbuf);

    *red   = pixel[(x*channel)+(y*width*channel)];
    *green = pixel[(x*channel)+(y*width*channel)+1];
    *blue  = pixel[(x*channel)+(y*width*channel)+2];
    *alpha = pixel[(x*channel)+(y*width*channel)+3];

    return TRUE;
}
