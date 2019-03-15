#include "cursor.h"
#include <gtk-3.0/gtk/gtk.h>

void resetCursor(SGlobalData* data)
{
    //init variables
    GdkDisplay *display = NULL;
    GdkCursor *cursor = NULL;
    GdkScreen *screen = NULL;
    GdkWindow * win = NULL;

    //set variables
    screen = gtk_window_get_screen(GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")));
    display = gdk_screen_get_display(screen);

    //create the new cursor
    cursor = gdk_cursor_new_from_name(display, "default");

    //gdk_display_beep (display); play a sound ("beep")

    //set the new cursor on the screen
    win = gdk_screen_get_root_window(screen);
    gdk_window_set_cursor (win, cursor);
}

void callback_setCursor(SGlobalData* data)
{
     //init variables
     
     GdkDisplay *display = NULL;
     GdkCursor *cursor = NULL;
     GdkScreen *screen = NULL;
     GdkWindow * win = NULL;
     struct _GdkPixbuf *imgPixbuf;
     struct _GdkPixbuf *img2;

     //set variables
     screen = gtk_window_get_screen(GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")));
     display = gdk_screen_get_display(screen);
     GError *error = NULL;
     imgPixbuf = gdk_pixbuf_new_from_file("penta00.gif", &error);
     int width = gdk_pixbuf_get_width(imgPixbuf);
     int height = gdk_pixbuf_get_height(imgPixbuf);
     img2 = gdk_pixbuf_scale_simple(imgPixbuf, width/6, height/6, GDK_INTERP_HYPER);

     //create the new cursor
     cursor = gdk_cursor_new_from_pixbuf(display, img2, 23, 23);
    if(error)
    {
        printf("Error : %s\n", error->message);
        g_error_free(error);
    }
     //gdk_display_beep (display); play a sound ("beep")

     //set the new cursor on the screen
     win = gdk_screen_get_root_window(screen);
     gdk_window_set_cursor (win, cursor);
 }

/*void resizeCursor(SGlobalData* data)
{
    //TODO
}

void stayInWindow(SGlobalData* data)
{
    //TODO
}*/
