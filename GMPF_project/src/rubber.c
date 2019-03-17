#include "rubber.h"

void square_rubber(GtkWidget *widget, cairo_t *cr, int x, int y, int size)
{
    cairo_set_source_rgba (cr, 1, 1, 1, 0);
    cairo_rectangle (cr, x - (size/2), y - (size/2), size, size);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_fill(cr);
    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area (widget, x - (size/2), y - (size/2), size, size);
}

void circular_rubber(GtkWidget *widget, cairo_t *cr, double x, double y, double radius)
{
    float pi = 3.1415926535;
    cairo_set_source_rgba (cr, 1, 1, 1, 0);
    cairo_arc(cr, x, y, radius, 0.0, 2 * pi);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_fill_preserve(cr);
    cairo_destroy (cr);

    gtk_widget_queue_draw_area (widget, x - radius, y - radius, radius*2, radius*2);
}
