#include "filters.h"


/*
 * Apply the "filter" function to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void GMPF_filter_apply_to_selected_layer(void (*filter)(GMPF_Layer*),
                                         gpointer       user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    {
        D_PRINT("Unable to get layer", NULL);
        return;
    }

    filter(lay);
    GMPF_saved_state_set_is_saved(flowbox, 0);

    gtk_widget_queue_draw(da);
}


/*
 * Apply the "filter" function to all Layer
 */
void GMPF_filter_apply_to_all_layer(void (*filter)(GMPF_Layer*),
                                    gpointer       user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            filter(lay);

            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    GMPF_saved_state_set_is_saved(flowbox, 0);

    gtk_widget_queue_draw(da);
}


/*
 * Apply the "filter" function to the selected Layer with the given "red",
 * "green" and "blue" parameters
 * (Do nothing if there is no selected Layer)
 */
void GMPF_filter_apply_to_selected_layer_color(void (*filter)(GMPF_Layer*,
                                                                        guchar,
                                                                        guchar,
                                                                        guchar),
                                                              guchar   r,
                                                              guchar   g,
                                                              guchar   b,
                                                              gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    {
        D_PRINT("Unable to get layer", NULL);
        return;
    }

    filter(lay, r, g, b);
    GMPF_saved_state_set_is_saved(flowbox, 0);

    gtk_widget_queue_draw(da);
}


/*
 * Apply the "filter" function to all Layer with the given "red", "green" and
 * "blue" parameters
 */
void GMPF_filter_apply_to_all_layer_color(void (*filter)(GMPF_Layer*,
                                                                        guchar,
                                                                        guchar,
                                                                        guchar),
                                                              guchar   r,
                                                              guchar   g,
                                                              guchar   b,
                                                              gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            filter(lay, r, g, b);

            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    GMPF_saved_state_set_is_saved(flowbox, 0);

    gtk_widget_queue_draw(da);
}


/*
 * Apply "Luminosité+" filter to the given Layer
 */
void Lightness(GMPF_Layer *lay)
{
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

            if (red > 230)
                red = 255;
            else
                red += 25;

            if (green > 230)
                green = 255;
            else
                green += 25;

            if (blue > 230)
                blue = 255;
            else
                blue += 25;
            put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);
}


/*
 * Apply "Luminosité-" filter to the given Layer
 */
void Darkness(GMPF_Layer *lay)
{
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

            if (red < 25)
                red = 0;
            else
                red -= 25;

            if (green < 25)
                green = 0;
            else
                green -= 25;

            if (blue < 25)
                blue = 0;
            else
                blue -= 25;
            put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);
}


/*
 * Apply "Nuance de gris" filter to the given Layer
 */
void Greyscale(GMPF_Layer *lay)
{
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
    layer_icon_refresh(lay);
}


/*
 * Apply "Negatif" filter to the given Layer
 */
void Negative(GMPF_Layer *lay)
{
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
    layer_icon_refresh(lay);
}


/*
 * Apply "Binarisation" filter to the given Layer
 */
void Binarize(GMPF_Layer *lay)
{
    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    guchar red, green, blue, alpha, grey;

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

            grey = (red + green + blue) / 3;
            if (grey > 127)
                grey = 255;
            else
                grey = 0;
            put_pixel(imgPixbuf, i, j, grey, grey, grey, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);

}


/*
 * Apply "Binarisation colorée" filter to the given Layer
 */
void BinarizeColor(GMPF_Layer *lay)
{
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

            if (red > 127)
                red = 255;
            else
                red = 0;

            if (green > 127)
                green = 255;
            else
                green = 0;

            if (blue > 127)
                blue = 255;
            else
                blue = 0;
            put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);
}


/*
 * Apply "Teinture" filter to the given Layer
 */
void Tinter(GMPF_Layer      *lay,
            GtkColorChooser *colorChooser)
{
    if (!lay)
        return;

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;
    guchar r, g, b, factor;
    GdkRGBA rgba;

    gtk_color_chooser_get_rgba (colorChooser, &rgba);
    r = (guchar)(rgba.red * 255);
    g = (guchar)(rgba.green * 255);
    b = (guchar)(rgba.blue * 255);
    //factor = (guchar)(rgba.alpha * 100);
    factor = 50;

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
    layer_icon_refresh(lay);
}


/*
 * Apply "Coloré" filter to the given Layer
 */
void Colorfull(GMPF_Layer      *lay,
               GtkColorChooser *colorChooser)
{
    if (!lay)
        return;

    guchar r, g, b, factor;
    GdkRGBA rgba;

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    gtk_color_chooser_get_rgba (colorChooser, &rgba);
    r = (guchar)(rgba.red * 255);
    g = (guchar)(rgba.green * 255);
    b = (guchar)(rgba.blue * 255);
    factor = (guchar)(rgba.alpha * 255);

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
    lay->surface = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 1, NULL);
    layer_icon_refresh(lay);

}


int check (int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i >= width || j >= height)
        return 0;
    return 1;
}


/*
 * Apply the given convolution matrix to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void Convolute(SGlobalData *data, double *mat)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;
    GMPF_saved_state_set_is_saved(flowbox, 0);

    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    double r, g, b, a;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    int x = 3;
    struct Img_rgb *img = init_img_rgb(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            r = g = b = a = 0;
            for (int k = -x / 2; k <= x/2; k++)
            {
                for(int l = -x / 2; l <= x/2; l++)
                {
                    if (check(width, height, i + k, j +l) == 1)
                    {
                        guchar red, green, blue, alpha;
                        error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i + k, j + l, &red, &green, &blue, &alpha);
                            if(!error)
                        err(1, "pixbuf get pixels error");
                        r += mat[l + x/2 + k + x/2] * (double)red;
                        g += mat[l + x/2 + k + x/2] * (double)green;
                        b += mat[l + x/2 + k + x/2] * (double)blue;
                        a = alpha;
                    }
                }
            }
            if (r > 255)
                r = 255;
            else if (r < 0)
                r = 0;

            if (g > 255)
                g = 255;
            else if (g < 0)
                g = 0;

            if (b > 255)
                b = 255;
            else if (b < 0)
                b = 0;

            Matrix_val(img -> red, i, j, r);
            Matrix_val(img -> green, i , j, g);
            Matrix_val(img -> blue, i , j, b);
            Matrix_val(img -> alpha, i, j, a);
        }
    }
    Img_rgb_to_Image(imgPixbuf, img);
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 1, NULL);
    layer_icon_refresh(lay);
    gtk_widget_queue_draw(da);
    free_img_rgb(img);
    free(mat);
}


/*
 * Apply "Egalisation" filter to the given Layer
 */
void Equalize(GMPF_Layer *lay)
{
    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    guchar r, g, b, a, grey;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    double *counter = calloc(sizeof(double), 256);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &r, &g, &b, &a);
            if(!error)
                err(1, "pixbuf get pixels error");

            grey = (r + g + b) / 3;
            *(counter + grey) += 1;
        }
    }

    int nb = 0;
    int cdfmin = 255;
    double *cdf = calloc(sizeof(double), 256);
    for (int i = 0; i < 256; i++)
    {
        nb += *(counter + i);
        *(cdf + i) = nb;
        if (*(cdf + i) < cdfmin)
            cdfmin = cdf[i];
    }

    int *h_v = calloc(sizeof(int), 256);
    double MNcdfmin = height * width - cdfmin;
    for (int i = 0; i < 256; i++)
    {
        *(h_v+i)=(int) (*(cdf+i) - cdfmin)/MNcdfmin * 255;
    }

    struct Img_rgb *img = init_img_rgb(width, height);
    for (int i = 0; i < width; i ++)
    {
        for (int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &r, &g, &b, &a);
            if(!error)
                err(1, "pixbuf get pixels error");

            grey = (r + g + b) / 3;

            Matrix_val(img -> red, i, j, *(h_v + grey));
            Matrix_val(img -> green, i, j, *(h_v + grey));
            Matrix_val(img -> blue, i, j, *(h_v + grey));
            Matrix_val(img -> alpha, i, j, a);
        }
    }

    Img_rgb_to_Image(imgPixbuf, img);
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 1, NULL);
    layer_icon_refresh(lay);
    free_img_rgb(img);
    free(counter);
    free(cdf);
    free(h_v);
}


/*
 * Apply "Mirroir verticale" filter to the given Layer
 */
void Verticale(GMPF_Layer *lay)
{
    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    guchar red, green, blue, alpha;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    struct Img_rgb *img = init_img_rgb(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");
			Matrix_val(img -> red, width - i - 1, j, (double)red);
            Matrix_val(img -> green, width - i - 1, j, (double)green);
            Matrix_val(img -> blue, width - i - 1, j, (double)blue);
            Matrix_val(img -> alpha, width - i - 1, j, (double)alpha);
            //put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    Img_rgb_to_Image(imgPixbuf, img);
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);
    free_img_rgb(img);
}


/*
 * Apply "Mirroir horizontale" filter to the given Layer
 */
void Horizontale(GMPF_Layer *lay)
{
    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    guchar red, green, blue, alpha;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    struct Img_rgb *img = init_img_rgb(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");
			Matrix_val(img -> red, i, height - j - 1, (double)red);
            Matrix_val(img -> green, i , height - j - 1, (double)green);
            Matrix_val(img -> blue, i, height - j - 1, (double)blue);
            Matrix_val(img -> alpha, i, height - j - 1, (double)alpha);
	}
    }
    Img_rgb_to_Image(imgPixbuf, img);
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);
    free_img_rgb(img);
}


/*
 * Apply "Egalisation colorée" filter to the given Layer
 */
void Equalize_color(GMPF_Layer *lay)
{
    g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    guchar r, g, b, a;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    double *counter_red = calloc(sizeof(double), 256);
	double *counter_green = calloc(sizeof(double), 256);
	double *counter_blue = calloc(sizeof(double), 256);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &r, &g, &b, &a);
            if(!error)
                err(1, "pixbuf get pixels error");

            *(counter_red + r) += 1;
            *(counter_green + g) += 1;
            *(counter_blue + b) += 1;
        }
    }

    int nb_red = 0;
    int nb_green = 0;
    int nb_blue = 0;

    int cdfmin_red = 255;
    int cdfmin_green = 255;
    int cdfmin_blue = 255;

    double *cdf_red = calloc(sizeof(double), 256);
    double *cdf_green = calloc(sizeof(double), 256);
    double *cdf_blue = calloc(sizeof(double), 256);
    for (int i = 0; i < 256; i++)
    {
        nb_red += *(counter_red + i);
        nb_green += *(counter_green + i);
        nb_blue += *(counter_blue + i);

        *(cdf_red + i) = nb_red;
        *(cdf_green + i) = nb_green;
        *(cdf_blue + i) = nb_blue;

        if (*(cdf_red + i) < cdfmin_red)
            cdfmin_red = cdf_red[i];
        if (*(cdf_green + i) < cdfmin_green)
            cdfmin_green = cdf_green[i];
        if (*(cdf_blue + i) < cdfmin_blue)
            cdfmin_blue = cdf_blue[i];
    }

    int *h_v_red = calloc(sizeof(int), 256);
    int *h_v_green = calloc(sizeof(int), 256);
    int *h_v_blue = calloc(sizeof(int), 256);

    double MNcdfmin_red = height * width - cdfmin_red;
    double MNcdfmin_green = height * width - cdfmin_green;
    double MNcdfmin_blue = height * width - cdfmin_blue;
    for (int i = 0; i < 256; i++)
    {
        *(h_v_red + i)= (int) (*(cdf_red+i) - cdfmin_red)/MNcdfmin_red * 255;
        *(h_v_green + i)= (int) (*(cdf_green+i) - cdfmin_green)/MNcdfmin_green * 255;
        *(h_v_blue + i)= (int) (*(cdf_blue+i) - cdfmin_blue)/MNcdfmin_blue * 255;
    }

    struct Img_rgb *img = init_img_rgb(width, height);
    for (int i = 0; i < width; i ++)
    {
        for (int j = 0; j < height; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &r, &g, &b, &a);
            if(!error)
                err(1, "pixbuf get pixels error");

            Matrix_val(img -> red, i, j, *(h_v_red + r));
            Matrix_val(img -> green, i, j, *(h_v_green + g));
            Matrix_val(img -> blue, i, j, *(h_v_blue + b));
            Matrix_val(img -> alpha, i, j, a);
        }
    }

    Img_rgb_to_Image(imgPixbuf, img);
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 1, NULL);
    layer_icon_refresh(lay);
    free_img_rgb(img);

    free(counter_red);
    free(counter_green);
    free(counter_blue);

    free(cdf_red);
    free(cdf_green);
    free(cdf_blue);

    free(h_v_red);
    free(h_v_green);
    free(h_v_blue);
}


/*
 * Apply a balance filter with the given "r", "g" and "b" parameters to the
 * given Layer
 */
void Color_balance(GMPF_Layer *lay, guchar r, guchar g, guchar b)
{
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

			if ((255 / r) * red > 255)
				red = 255;
			else
				red = (255 / r) * red;

			if ((255 / g) * green > 255)
				green = 255;
			else
				green = (255 / g) * green;

			if ((255 / b) * blue > 255)
				blue = 255;
			else
				blue = (255 / b) * blue;

            put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);
}
