#include "callback.h"

#ifndef RUBBER_H
#define RUBBER_H

void square_rubber(GtkWidget *widget, cairo_t *cr, int x, int y, int size);
void circular_rubber(GtkWidget *widget, cairo_t *cr, double x, double y, double radius);

#endif
