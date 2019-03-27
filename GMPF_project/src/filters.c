#include "filters.h"

void Greyscale(SGlobalData *data)
{
    GtkWidget *da = GET_UI(GtkWidget, "drawingArea");
    GtkFlowBox *flowbox = GET_UI(GtkFlowBox, "GMPF_flowbox");

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    guchar red, green, blue, alpha;
    guchar grey;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");
            grey = (red + green + blue)/3;
            put_pixel(imgPixbuf, i, j, grey, grey, grey, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    gtk_widget_queue_draw(da);
}

void Negative(SGlobalData *data)
{
    GtkWidget *da = GET_UI(GtkWidget, "drawingArea");

    GtkFlowBox *flowbox = GET_UI(GtkFlowBox, "GMPF_flowbox");

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    guchar red, green, blue, alpha;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
            if(!error)
            err(1, "pixbuf get pixels error");
            red = 255 - red;
            green = 255 - green;
            blue = 255 - blue;
            put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    gtk_widget_queue_draw(da);
}

void Binarize(SGlobalData *data)
{
    GtkFlowBox *flowbox =
        (GtkFlowBox*)(gtk_builder_get_object(data -> builder, "GMPF_flowbox"));

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    int width = (lay -> size).w;
    int height = (lay -> size).h;
    // int img_height = cairo_image_surface_get_height(lay->surface);
    // int stride = cairo_image_surface_get_stride(lay->surface);
    // unsigned char *buffer;
    // buffer = cairo_image_surface_get_data(lay->surface);
    // if (buffer)
    //     D_PRINT("buffer content :%c\n", buffer[10]);
    // for (int i = 0; i < img_height; i++)
    //     for (int j = 0; j < stride * 4; j += 4)
    //     {
    //         D_PRINT("i: %d, j: %d\n", i, j);
    //         buffer[i + j * stride] = 0xFF;
    //         buffer[i + j * stride + 1] = 0xFF;
    //         buffer[i + j * stride + 2] = 0xFF;
    //         buffer[i + j * stride + 3] = 0xFF;
    //     }
    //
    //
    // lay->surface = cairo_image_surface_create_for_data(buffer, CAIRO_FORMAT_ARGB32, lay->size.w, lay->size.h, stride);
    // g_free(buffer);

    struct GMPF_Pos *pos = malloc(sizeof(struct GMPF_Pos));
    struct GMPF_Pixel *pixel = malloc(sizeof(struct GMPF_Pixel));

    cairo_t *cr = cairo_create(lay -> surface);

    unsigned long grey;
    //lay -> cr = cairo_create(lay -> surface);

    for(int i = 0; i < width; i++)
    {
        pos -> x = i;
        for(int j = 0; j < height; j++)
        {
            pos -> y = j;

            if (layer_get_pixel(lay, pos, pixel) != 0)
                errx(EXIT_FAILURE, "error get pixel");

            grey = (pixel->R + pixel->G + pixel->B)/3;
            if (grey > 127)
                grey = 255;
            else
                grey = 0;

            /*pixel -> R = grey;
            pixel -> G = grey;
            pixel -> B = grey;
            if (layer_put_pixel(lay, pos, pixel) == 0)
                printf("%d %d %d \n", pixel -> R, pixel -> G, pixel -> G);
*/
            cairo_set_source_rgba(cr, grey,grey,grey, pixel->A);
            cairo_move_to(cr, i, j);
            cairo_rel_line_to(cr, 0, 1);
            cairo_stroke(cr);
        }
    }
    GtkWidget *w = GET_UI(GtkWidget, "drawingArea");
    gtk_widget_queue_draw(w);
    cairo_destroy(cr);
    free(pos);
    free(pixel);
}

void BinarizeColor(SGlobalData *data)
{
    GtkFlowBox *flowbox =
        (GtkFlowBox*)(gtk_builder_get_object(data -> builder, "GMPF_flowbox"));

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    int width = (lay -> size).w;
    int height = (lay -> size).h;

    struct GMPF_Pos *pos = malloc(sizeof(struct GMPF_Pos));
    struct GMPF_Pixel *pixel = malloc(sizeof(struct GMPF_Pixel));

    cairo_t *cr = cairo_create(lay -> surface);

    unsigned long red, green, blue;
    //lay -> cr = cairo_create(lay -> surface);

    for(int i = 0; i < width; i++)
    {
        pos -> x = i;
        for(int j = 0; j < height; j++)
        {
            pos -> y = j;

            if (layer_get_pixel(lay, pos, pixel) != 0)
                errx(EXIT_FAILURE, "error get pixel");

			if (pixel -> R > 127)
				red = 255;
			else
				red = 0;

			if (pixel -> B > 127)
				blue = 255;
			else
				blue = 0;

			if (pixel -> G > 127)
				green = 255;
			else
				green = 0;

            cairo_set_source_rgba(cr, red, green, blue, pixel->A);
            cairo_move_to(cr, i, j);
            cairo_rel_line_to(cr, 0, 1);
            cairo_stroke(cr);
        }
    }
    GtkWidget *w = GET_UI(GtkWidget, "drawingArea");
    gtk_widget_queue_draw(w);
    cairo_destroy(cr);
    free(pos);
    free(pixel);
}

void Tinter(SGlobalData *data)
{
    GtkColorChooser *colorChooser = NULL;
    GtkWidget *da = GET_UI(GtkWidget, "drawingArea");

    GtkFlowBox *flowbox =
        (GtkFlowBox*)(gtk_builder_get_object(data -> builder, "GMPF_flowbox"));

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;
    guchar r, g, b, factor;
    GdkRGBA rgba;

    colorChooser = (GtkColorChooser *)(gtk_builder_get_object(data->builder, "ColorTinter"));
    gtk_color_chooser_get_rgba (colorChooser, &rgba);
    r = (guchar)(rgba.red * 255);
    g = (guchar)(rgba.green * 255);
    b = (guchar)(rgba.blue * 255);
    //factor = (guchar)(rgba.alpha * 100);
    factor = 25;

    guchar red;
    guchar green;
    guchar blue, alpha;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
            if(!error)
            err(1, "pixbuf get pixels error");
            red = red * (100 - factor) / 100 + r * factor / 100;
            green = green * (100 - factor) / 100 + g * factor / 100;
            blue = blue * (100 - factor) / 100 + b * factor / 100;
            put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    gtk_widget_queue_draw(da);
}
