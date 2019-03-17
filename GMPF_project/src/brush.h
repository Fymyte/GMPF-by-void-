#include "callback.h"

#ifndef BRUSH_H
#define BRUSH_H

void square_brush(GtkWidget *widget, cairo_t *cr, int x, int y, int size);
void circular_brush(GtkWidget *widget, cairo_t *cr, double x, double y, double radius);

#endif
