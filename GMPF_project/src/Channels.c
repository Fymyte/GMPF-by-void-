#include "Channels.h"

extern SGlobalData G_user_data;

/*
 * function to eraise the selected color with the given tolerance
 */
void kill_color(GMPF_Layer *layer,
                double      tolerance)
{
    GET_UI (GtkColorChooser, colorbtn, "ColorKillerChooser");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    if (!layer)
        return;

    if (layer->image)
        g_object_unref(layer->image);
    layer->image = gdk_pixbuf_get_from_surface(layer->surface, 0, 0, layer->size.w, layer->size.h);
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
}

void swap_color(GMPF_Layer *layer)
{
    GET_UI(GtkColorChooser, colorbtn1, "ColorSwapperChooser1");
    GET_UI(GtkColorChooser, colorbtn2, "ColorSwapperChooser2")
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    if (!layer)
        return;

    if (layer->image)
        g_object_unref(layer->image);
    layer->image = gdk_pixbuf_get_from_surface(layer->surface, 0, 0, layer->size.w, layer->size.h);
    int width = layer->size.w;
    int height = layer->size.h;
    GdkRGBA color1;
    GdkRGBA color2;
    gtk_color_chooser_get_rgba(colorbtn1, &color1);
    gtk_color_chooser_get_rgba(colorbtn2, &color2);

    guchar red, green, blue, alpha;
    gboolean error = FALSE;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(layer->image, i, j, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");

            if ((red == ((color1.red)*255))
                && (blue == ((color1.blue)*255))
                && (green == ((color1.green)*255)))
                {
                    put_pixel(layer->image, i, j, color2.red * 255, color2.green * 255, color2.blue * 255, color2.alpha * 255 );
                }
        }
    }
    cairo_surface_destroy(layer->surface);
    layer->surface = gdk_cairo_surface_create_from_pixbuf(layer->image, 1, NULL);
    layer_icon_refresh(layer);
}
