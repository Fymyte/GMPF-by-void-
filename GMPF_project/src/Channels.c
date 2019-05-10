#include "Channels.h"

extern SGlobalData G_user_data;

/*
 * function to eraise the selected color with the given tolerance
 */
void kill_color(GtkWidget *widget,
                double     tolerance)
{
    GET_UI (GtkColorChooser, colorbtn, "ColorTinter");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    if (!layer)
        return;
    int width = layer->size.w;
    int height = layer->size.h;
    GdkRGBA color;
    gtk_color_chooser_get_rgba(colorbtn, &color);

    guchar red, green, blue, alpha;
    double k = tolerance; //tolerance de pixels
    gboolean error = FALSE;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(layer->image, i, j, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");

            if ((red <= ((color.red + k)*255) && red >= ((color.red - k)*255))
                && (blue <= ((color.blue + k)*255) && blue >= ((color.blue - k)*255))
                && (green <= ((color.green + k)*255) && green >= ((color.green - k)*255)))
                {
                    put_pixel(layer->image, i, j, red, green, blue, 0);
                }
        }
    }
    cairo_surface_destroy(layer->surface);
    layer->surface = gdk_cairo_surface_create_from_pixbuf(layer->image, 1, NULL);
    layer_icon_refresh(layer);
    gtk_widget_queue_draw(widget);
}
