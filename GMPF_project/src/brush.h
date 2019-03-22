#include "callback.h"

#ifndef BRUSH_H
#define BRUSH_H

void square_brush(GtkWidget *widget, cairo_t *cr, int x, int y, int size,
                        float red, float green, float blue, float alpha);
void circular_brush(GtkWidget *widget, cairo_t *cr, double x, double y,
                double radius, float red, float green, float blue, float alpha,
                float scale_x, float scale_y);
#endif
