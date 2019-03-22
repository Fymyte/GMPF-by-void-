#include "brush.h"

void square_brush(GtkWidget *widget, cairo_t *cr, int x, int y, int size,
                        float red, float green, float blue, float alpha)
{
    int semi_size = size >> 1;
    int nx = x - semi_size;
    int ny = y - semi_size;
    cairo_set_source_rgba (cr, red, green, blue, alpha);
    cairo_rectangle (cr, nx, ny, size, size);

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill(cr);
    // cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area (widget, nx, ny, size, size);
}

void circular_brush(GtkWidget *widget, cairo_t *cr, double x, double y,
                double radius, float red, float green, float blue, float alpha,
                float scale_x, float scale_y)
{
    // float two_pi = 6.2831853070; // float pi = 3.1415926535;
    cairo_set_source_rgba (cr, red, green, blue, alpha);
    cairo_arc(cr, x / scale_x, y / scale_y, radius, 0.0, G_PI * 2);
    D_PRINT("x: %f, y: %f\n", x, y);

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill_preserve(cr);
    // cairo_destroy (cr);
    // gtk_widget_queue_draw(widget);
    gtk_widget_queue_draw_area (widget, x - radius, y - radius, radius*2, radius*2);
}
