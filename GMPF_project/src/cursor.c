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

/*void resizeCursor(SGlobalData* data)
{
    //TODO
}

void stayInWindow(SGlobalData* data)
{
    //TODO
}*/
