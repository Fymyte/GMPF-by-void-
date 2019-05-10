#include "callback.h"

#ifndef CURSOR_H
#define CURSOR_H

#include <gtk-3.0/gtk/gtk.h>

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;

void pick_color_on_screen(int x,
                          int y);
void resetCursor(GtkWindow *window);
void stayInWindow();
void resizeCuror(int size);
void callback_setCursor();

#endif
