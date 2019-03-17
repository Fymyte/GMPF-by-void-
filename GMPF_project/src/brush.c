#include "brush.h"

void square_brush(GtkWidget *widget, cairo_t *cr, int x, int y, int size)
{
    cairo_set_source_rgba (cr, 1, 0, 0, 1);
    cairo_rectangle (cr, x - (size/2), y - (size/2), size, size);
    cairo_fill(cr);
    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area (widget, x - (size/2), y - (size/2), size, size);
}

void circular_brush(GtkWidget *widget, cairo_t *cr, double x, double y, double radius)
{
    float pi = 3.1415926535;
    cairo_set_source_rgba (cr, 1, 0, 0, 1);
    cairo_arc(cr, x, y, radius, 0.0, 2 * pi);
    cairo_fill_preserve(cr);
    cairo_destroy (cr);
    gtk_widget_queue_draw_area (widget, x - radius, y - radius, radius*2, radius*2);
}
