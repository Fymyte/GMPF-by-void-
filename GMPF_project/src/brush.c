#include "brush.h"

void square_brush(GtkWidget *widget, cairo_t *cr, int x, int y, int size)
{
    int semi_size = size >> 1;
    int nx = x - semi_size;
    int ny = y - semi_size;
    cairo_set_source_rgba (cr, 1, 0, 0, 1);
    cairo_rectangle (cr, nx, ny, size, size);

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill(cr);
    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area (widget, nx, ny, size, size);
}

void circular_brush(GtkWidget *widget, cairo_t *cr, double x, double y, double radius)
{
    float two_pi = 6.2831853070; // float pi = 3.1415926535;
    cairo_set_source_rgba (cr, 1, 0, 0, 1);
    cairo_arc(cr, x, y, radius, 0.0, two_pi);

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill_preserve(cr);
    cairo_destroy (cr);
    gtk_widget_queue_draw_area (widget, x - radius, y - radius, radius*2, radius*2);
}
