#ifndef CURSOR_H
#define CURSOR_H

#include <gtk-3.0/gtk/gtk.h>

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;

void resetCursor(SGlobalData* data);
void stayInWindow(SGlobalData* data);
void resizeCuror(SGlobalData* data);

#endif 
