#include "brush.h"

extern SGlobalData G_user_data;

/*
 * PURPOSE : Make a square collored shape collored at the given position
 *  PARAMS : GtkWidget *widget -
 *           cairo_t   *cr - cairo context associated with the surface to paint on
 *           int        x - x position to paint
 *           int        y - y position to paint
 *           int        size - size of the brush
 *           float      red - red channel for brush
 *           float      green - green channel for brush
 *           float      blue - blue channel for brush
 *           float      alpha - alpha channel for brush
 * RETURNS : None
 *   NOTES : affect only the surface associated with the given cairo context
 */
void square_brush(GtkWidget *widget,
                  cairo_t   *cr,
                  int        x,
                  int        y,
                  int        size,
                  float      red,
                  float      green,
                  float      blue,
                  float      alpha,
                  float      scale_x,
                  float      scale_y)
{
    size = size << 1;
    int semi_size = size >> 1;
    int nx = x - semi_size;
    int ny = y - semi_size;
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
    cairo_set_source_rgba (cr, red, green, blue, alpha);
    cairo_rectangle (cr, nx / scale_x, ny/scale_y, size, size);

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill(cr);
    // cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw (widget);
}

/*
 * PURPOSE : Make a rectangle collored shape collored at the given position
 *  PARAMS : GtkWidget *widget -
 *           cairo_t   *cr - cairo context associated with the surface to paint on
 *           int        x - x position to paint
 *           int        y - y position to paint
 *           int        size - size of the brush
 *           float      red - red channel for brush
 *           float      green - green channel for brush
 *           float      blue - blue channel for brush
 *           float      alpha - alpha channel for brush
 * RETURNS : None
 *   NOTES : affect only the surface associated with the given cairo context
 */
void rectangular_brush(GtkWidget *widget,
                  cairo_t   *cr,
                  int        x,
                  int        y,
                  int        size,
                  float      red,
                  float      green,
                  float      blue,
                  float      alpha,
                  float      scale_x,
                  float      scale_y)
{
    size = size << 1;
    int semi_size = size >> 1;
    int nx = x - semi_size;
    int ny = y - semi_size;

    cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
    cairo_set_source_rgba (cr, red, green, blue, alpha);
    cairo_rectangle (cr, nx / scale_x, ny / scale_y, size, (size/2));

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill(cr);
    // cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw (widget);
}

void triangle_brush(GtkWidget *widget,
                  cairo_t   *cr,
                  int        x,
                  int        y,
                  int        size,
                  float      red,
                  float      green,
                  float      blue,
                  float      alpha)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    size = size << 1;
    int semi_size = size >> 1;
    int nx = x - semi_size;
    int ny = y - semi_size;

    cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
    GMPF_Scale scale = *GMPF_project_info_get_scale(flowbox);


    cairo_set_line_width(cr, 1);
    cairo_move_to(cr, x/scale.x, ny/scale.y);
    cairo_line_to(cr, nx*scale.x, (ny*scale.y + size));
    cairo_line_to(cr, (nx*scale.x + size), (y*scale.y + semi_size));
    cairo_close_path (cr);

    cairo_set_source_rgba (cr, red, green, blue, alpha);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_fill(cr);
    // cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw (widget);
}

/*
 * Make a circle shape collored with "red", "green", "blue" and "alpha"
 * parameters at the given position
 * (If there is multiple points, draw a line between the LayerMngr's point and
 * the given coordinates, update le LayerMngr's point to the given coordinates)
 * (affect only the surface associated with the given cairo context)
 */
void circular_brush(GtkWidget      *widget,
                    cairo_t        *cr,
                    double          x,
                    double          y,
                    double          radius,
                    float           red,
                    float           green,
                    float           blue,
                    float           alpha,
                    float           scale_x,
                    float           scale_y,
                    GMPF_LayerMngr *layermngr)
{
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

        refreshx = (x < layermngr->pos.x ? x : layermngr->pos.x) - (radius * scale_x);
        refreshy = (y < layermngr->pos.y ? y : layermngr->pos.y) - (radius * scale_y);
        width = (x - layermngr->pos.x) * 2;
        height = (y - layermngr->pos.y) * 2;
        if (width < 0)
            width = (width - radius * 2) * -1;
        else
            width += radius * 2;
        if (height < 0)
            height = (height - radius * 2) * -1;
        else
            height += radius * 2;
    }
    else
    {
        cairo_arc(cr, x / scale_x, y / scale_y, radius, 0.0, G_PI * 2);
        cairo_fill_preserve(cr);
        refreshx = x - (radius * scale_x);
        refreshy = y - (radius * scale_y);
        width = 2 * radius;
        height = 2 * radius;
    }

    layermngr->pos.x = x;
    layermngr->pos.y = y;

    gtk_widget_queue_draw(widget);
}


// TODO
/*
 * PURPOSE : Copy the selected area inside of a new surface with it's given
 * position on the surface and update the LayerMngr's point
 *  PARAMS : UNUSED GtkWidget *widget -
 *           double             x - x positon to select
 *           double             y - y position to select
 *           GMPF_LayerMngr    *layermngr -
 * RETURNS : None
 *   NOTES :
 */
char selector(GtkFlowBox *flowbox,
              GMPF_Pos    pos,
              GMPF_Pos    npos)
{
    GMPF_Size size = { .w = 0, .h = 0};

    if (pos.x == npos.x || pos.y == npos.y)
        return 1;

    if (pos.x < npos.x)
    { size.w = npos.x - pos.x; npos.x = pos.x; }
    else
    { size.w = pos.x - npos.x; pos.x = npos.x; }

    if (pos.y < npos.y)
    { size.h = npos.y - pos.y; npos.y = pos.y; }
    else
    { size.h = pos.y - npos.y; pos.y = npos.y; }

    GMPF_selection_set_pos(flowbox, pos);
    GMPF_selection_set_size(flowbox, size);

    return 0;
}
