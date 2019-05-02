#include "cursor.h"
//#include "GMPF_LayerMngr.h"
//#include <gtk-3.0/gtk/gtk.h>

/*
 * Take the color of the selected Layer at the given coordinates and put the
 * taken color inside of the colorChooser's color parameter
 */
void pick_color_on_screen(int              x,
                          int              y,
                          GtkColorChooser *colorChooser,
                          gpointer         user_data)
{
    INIT_UI();
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return;
    float scale_x = lay->scale_factor.x;
    float scale_y = lay->scale_factor.y;
    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);
    GdkRGBA *color = malloc(sizeof(GdkRGBA));
    guchar r, g, b, a;
    gdkpixbuf_get_colors_by_coordinates(pixbuf, x / scale_x, y / scale_y, &r, &g, &b, &a);
    color->red = (gdouble)r / 255;
    color->green = (gdouble)g / 255;
    color->blue = (gdouble)b / 255;
    color->alpha = (gdouble)a / 255;
    gtk_color_chooser_set_rgba(colorChooser, color);
    g_free(color);
    g_object_unref(pixbuf);
}


/*
 * Reset the cusror icon to it'sn system default icon
 */
void resetCursor(GtkWindow *window)
{
    //init variables
    GdkDisplay *display = NULL;
    GdkCursor *cursor = NULL;
    GdkScreen *screen = NULL;
    GdkWindow * win = NULL;

    //set variables
    screen = gtk_window_get_screen(window);
    display = gdk_screen_get_display(screen);

    //create the new cursor
    cursor = gdk_cursor_new_from_name(display, "default");

    //gdk_display_beep (display); play a sound ("beep")

    //set the new cursor on the screen
    win = gdk_screen_get_root_window(screen);
    gdk_window_set_cursor (win, cursor);
}


/*
 * Set the cusror to a circle penta cursor icon
 */
void callback_setCursor(SGlobalData* data)
 {
      //init variables

      GdkDisplay *display = NULL;
      GdkCursor *cursor = NULL;
      GdkScreen *screen = NULL;
      GdkWindow * win = NULL;
      GtkFlowBox *flowbox;
      GMPF_LayerMngr *layermngr;
      int size;
      cairo_t *cr;

      //set variables
      screen = gtk_window_get_screen(GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")));
      display = gdk_screen_get_display(screen);
      flowbox = (GtkFlowBox *)gtk_builder_get_object(data->builder, "GMPF_flowbox");
      layermngr = layermngr_get_layermngr(flowbox);
      size = layermngr->brush_size;
      cairo_surface_t *cairo_cursor =
            cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size * 2.0, size * 2.0);
      cr = cairo_create(cairo_cursor);

      //create the new cursor

      cairo_set_line_width(cr, 0.5);

      //circle
      if (layermngr->brush == 0)
      {
          cairo_arc (cr, size, size, size, 0.0, G_PI * 2.0);
      }

      //square
      if (layermngr->brush == 1)
      {
          cairo_rectangle(cr, 0, 0, size*2.0, size*2.0);
      }

      cairo_set_source_rgba (cr, 0, 0, 0, 1);
      cairo_stroke (cr);
      cairo_cursor = cairo_get_target(cr);
      cursor = gdk_cursor_new_from_surface (display, cairo_cursor, size, size);

      cairo_surface_destroy(cairo_cursor);
      cairo_destroy(cr);
      //gdk_display_beep (display); play a sound ("beep")

      //set the new cursor on the screen
      win = gdk_screen_get_root_window(screen);
      gdk_window_set_cursor (win, cursor);
  }


/*
 * Resize the cursor to math the LayerMngr's cursor's size parameter
 */
void resizeCursor(SGlobalData* data, int size)
{
    GdkWindow * win = NULL;
    GdkCursor *cursor = NULL;
    struct _GdkPixbuf *pixbuf;
    struct _GdkPixbuf *newpixbuf;
    GdkScreen *screen = NULL;
    GdkDisplay *display = NULL;

    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file("penta00.gif", &error);
    if(error)
    {
        printf("Error : %s\n", error->message);
        g_error_free(error);
    }

    screen = gtk_window_get_screen(GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")));
    display = gdk_screen_get_display(screen);
    win = gdk_screen_get_root_window(screen);

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    newpixbuf = gdk_pixbuf_scale_simple(pixbuf, (width*size)/100, (height*size)/100, GDK_INTERP_HYPER);
    cursor = gdk_cursor_new_from_pixbuf(display, newpixbuf, (width*size)/200, (height*size)/200);

    gdk_window_set_cursor(win, cursor);
}
