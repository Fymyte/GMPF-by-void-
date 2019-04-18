#include "Channels.h"

void kill_color(GtkWidget *widget, gpointer user_data)
{
    INIT_UI();
    GET_UI (GtkColorChooser, colorbtn, "ColorTinter");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    int width = layer->size.w;
    int height = layer->size.h;
    GdkRGBA color;
    gtk_color_chooser_get_rgba(colorbtn, &color);

    guchar red, green, blue, alpha;
    gboolean error = FALSE;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(layer->image, i, j, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");

            if (red == color.red /*&& blue == color.blue && green == color.green && color.alpha == alpha*/)
                put_pixel(layer->image, i, j, 255, 255, 255, 255);
        }
    }
    cairo_surface_destroy(layer->surface);
    layer->surface = gdk_cairo_surface_create_from_pixbuf(layer->image, 1, NULL);
    layer_icon_refresh(layer);
    gtk_widget_queue_draw(widget);
    printf("done\n");
}
