#include "brush.h"

void square_brush(GtkWidget *widget, cairo_t *cr, int x, int y, int size,
                        float red, float green, float blue, float alpha)
{
    int semi_size = size >> 1;
    int nx = x - semi_size;
    int ny = y - semi_size;
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
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
                float scale_x, float scale_y, GMPF_LayerMngr *layermngr)
{
    // float two_pi = 6.2831853070; // float pi = 3.1415926535;
    float refreshx, refreshy;
    int width, height;
    cairo_set_source_rgba (cr, red, green, blue, alpha);
    if (layermngr->pos.x != -1)
    {
        cairo_set_line_width(cr, radius * 2);
        cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_move_to(cr, layermngr->pos.x / scale_x, layermngr->pos.y / scale_y);
        cairo_line_to(cr, x / scale_x, y / scale_y);
        cairo_stroke(cr);

        refreshx = (x < layermngr->pos.x ? x : layermngr->pos.x) - radius;
        refreshy = (y < layermngr->pos.y ? y : layermngr->pos.y) - radius;
        width = x - layermngr->pos.x;
        height = y - layermngr->pos.y;
        if (width < 0)
            width = (width - radius) * -1;
        else
            width += radius;
        if (height < 0)
            height = (height - radius) * -1;
        else
            height += radius;
    }
    else
    {
        cairo_arc(cr, x / scale_x, y / scale_y, radius, 0.0, G_PI * 2);
        cairo_fill_preserve(cr);
        refreshx = x - radius;
        refreshy = y - radius;
        width = 2 * radius;
        height = 2 * radius;
    }

    // cairo_destroy (cr);
    // gtk_widget_queue_draw(widget);
    D_PRINT("x: %f, y: %f, rx: %f, ry: %f, w: %d, h: %d, radius: %f\n", x, y, refreshx, refreshy, width, height, radius);

    layermngr->pos.x = x;
    layermngr->pos.y = y;
    // gtk_widget_queue_draw(da);
    gtk_widget_queue_draw_area (widget, refreshx, refreshy,
            width, height);
}
