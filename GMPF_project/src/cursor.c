#include "cursor.h"

extern SGlobalData G_user_data;

/*
 * Take the color of the selected Layer at the given coordinates and put the
 * taken color inside of the colorChooser's color parameter
 */
void pick_color_on_screen(int x,
                          int y)
{
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkColorChooser, colorChooser, "ColorTinter");
    GET_UI(GtkColorChooser, colorKiller, "ColorKillerChooser");
    GET_UI(GtkColorChooser, colorSwapper, "ColorSwapperChooser1");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return;
    x = (x)/lay->scale_factor.x;
    y = (y)/lay->scale_factor.y;
    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(lay->surface, -lay->pos.x, -lay->pos.y, lay->size.w, lay->size.h);
    GdkRGBA *color = malloc(sizeof(GdkRGBA));
    guchar r, g, b, a;
    gdkpixbuf_get_colors_by_coordinates(pixbuf, x, y, &r, &g, &b, &a);
    color->red = (gdouble)r / 255;
    color->green = (gdouble)g / 255;
    color->blue = (gdouble)b / 255;
    color->alpha = (gdouble)a / 255;
    gtk_color_chooser_set_rgba(colorChooser, color);
    gtk_color_chooser_set_rgba(colorKiller, color);
    gtk_color_chooser_set_rgba(colorSwapper, color);
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
void callback_setCursor()
 {
      //init variables

      GdkDisplay *display = NULL;
      GdkCursor *cursor = NULL;
      GdkScreen *screen = NULL;
      GdkWindow * win = NULL;
      GMPF_LayerMngr *layermngr;
      int size;
      cairo_t *cr;

      //set variables
      GET_UI(GtkWindow, window, "MainWindow");
      GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
      screen = gtk_window_get_screen(window);
      display = gdk_screen_get_display(screen);

      layermngr = layermngr_get_layermngr(flowbox);
      size = layermngr->brush_size;
      cairo_surface_t *cairo_cursor =
            cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size * 2.0, size * 2.0);
      cr = cairo_create(cairo_cursor);

      //create the new cursor



      //circle
      if (layermngr->brush == 0)
      {
          cairo_set_line_width(cr, 0.5);
          cairo_arc (cr, size, size, size, 0.0, G_PI * 2.0);
      }

      //square
      if (layermngr->brush == 1)
      {
          cairo_set_line_width(cr, 1);
          cairo_rectangle(cr, 0, 0, size*2.0, size*2.0);
      }

      //rectangle
      if (layermngr->brush == 2)
      {
          cairo_set_line_width(cr, 1);
          cairo_rectangle(cr, 0, 0, size*2.0, size);
      }

      //triangle
      if (layermngr->brush == 3)
      {
          cairo_set_line_width(cr, 1);
          cairo_move_to(cr, size, 0);
          cairo_line_to(cr, 0, size*2.0);
          cairo_line_to(cr, size*2.0, size*2.0);
          cairo_close_path (cr);
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
void resizeCursor(int size)
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
    GET_UI(GtkWindow, window, "MainWindow");
    screen = gtk_window_get_screen(window);
    display = gdk_screen_get_display(screen);
    win = gdk_screen_get_root_window(screen);

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    newpixbuf = gdk_pixbuf_scale_simple(pixbuf, (width*size)/100, (height*size)/100, GDK_INTERP_HYPER);
    cursor = gdk_cursor_new_from_pixbuf(display, newpixbuf, (width*size)/200, (height*size)/200);

    gdk_window_set_cursor(win, cursor);
}
