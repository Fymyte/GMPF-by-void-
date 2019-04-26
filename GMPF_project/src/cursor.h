#include "callback.h"

#ifndef CURSOR_H
#define CURSOR_H

#include <gtk-3.0/gtk/gtk.h>

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;

void pick_color_on_screen(int              x,
                          int              y,
                          GtkColorChooser *colorChooser,
                          gpointer         user_data);
void resetCursor(SGlobalData* data);
void stayInWindow(SGlobalData* data);
void resizeCuror(SGlobalData* data, int size);
void callback_setCursor(SGlobalData* data);

#endif
