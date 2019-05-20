#include "filters.h"

extern SGlobalData G_user_data;

int check (int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i >= width || j >= height)
        return 0;
    return 1;
}


void filter_for_selection_unop(void(*filter)(GMPF_Layer *), GtkFlowBox *flowbox)
{
    cairo_surface_t *new_surf = GMPF_selection_get_surface(flowbox);
    if (!new_surf)
    { PRINTERR("No surface"); return; }

    while (cairo_surface_get_reference_count(new_surf) < 3)
    { D_PRINT("Reference", NULL); cairo_surface_reference(new_surf); }

    GMPF_Size size = *GMPF_selection_get_size(flowbox);
    GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);

    GMPF_Layer *selec_lay = layer_initialization();
    selec_lay->surface = new_surf;
    selec_lay->size.w = size.w;
    selec_lay->size.h = size.h;

    selec_lay->pos.x = pos.x;
    selec_lay->pos.y = pos.y;

    //filter selection
    filter(selec_lay);
    //end filter selection
    GMPF_selection_set_surface(flowbox, selec_lay->surface);

    if (selec_lay->image)
        g_object_unref(selec_lay->image);
    free(selec_lay);
}


void filter_for_selection(void(*filter)(GMPF_Pixel *), GtkFlowBox *flowbox)
{
    cairo_surface_t *new_surf = GMPF_selection_get_surface(flowbox);
    if (!new_surf)
    { PRINTERR("No surface"); return; }

    while (cairo_surface_get_reference_count(new_surf) < 3)
    { D_PRINT("Reference", NULL); cairo_surface_reference(new_surf); }

    GMPF_Size size = *GMPF_selection_get_size(flowbox);
    GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);

    GMPF_Layer *selec_lay = layer_initialization();
    selec_lay->surface = new_surf;
    selec_lay->size.w = size.w;
    selec_lay->size.h = size.h;

    selec_lay->pos.x = pos.x;
    selec_lay->pos.y = pos.y;

    //filter selection
    pixelFilter(selec_lay, filter);
    //end filter selection
    GMPF_selection_set_surface(flowbox, selec_lay->surface);

    if (selec_lay->image)
        g_object_unref(selec_lay->image);
    free(selec_lay);
}


void filter_for_selection_color(void (*filter)(GMPF_Layer*,
                                                           guchar,
                                                           guchar,
                                                           guchar),
                                                 guchar   r,
                                                 guchar   g,
                                                 guchar   b,
                                                 GtkFlowBox *flowbox)
{
    cairo_surface_t *new_surf = GMPF_selection_get_surface(flowbox);
    if (!new_surf)
    { PRINTERR("No surface"); return; }

    while (cairo_surface_get_reference_count(new_surf) < 3)
    { D_PRINT("Reference", NULL); cairo_surface_reference(new_surf); }

    GMPF_Size size = *GMPF_selection_get_size(flowbox);
    GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);

    GMPF_Layer *selec_lay = layer_initialization();
    selec_lay->surface = new_surf;
    selec_lay->size.w = size.w;
    selec_lay->size.h = size.h;

    selec_lay->pos.x = pos.x;
    selec_lay->pos.y = pos.y;


    //filter selection
    filter(selec_lay, r, g, b);
    //end filter selection
    GMPF_selection_set_surface(flowbox, selec_lay->surface);

    free(selec_lay);
}


/*
 * Apply the "filter" function to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void GMPF_filter_apply_to_selected_layer_unop(void (*filter)(GMPF_Layer*))
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    {
        D_PRINT("Unable to get layer", NULL);
        return;
    }

    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
    filter(lay);
    GMPF_saved_state_set_is_saved(flowbox, 0);

    gtk_widget_queue_draw(da);
}


/*
 * Apply the "filter" function to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void GMPF_filter_apply_to_selected_layer(void (*filter)(GMPF_Pixel*))
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    {
        D_PRINT("Unable to get layer", NULL);
        return;
    }

    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
    pixelFilter(lay, filter);
    GMPF_saved_state_set_is_saved(flowbox, 0);

    gtk_widget_queue_draw(da);
}


/*
 * Apply the "filter" function to all Layer
 */
void GMPF_filter_apply_to_all_layer_unop(void (*filter)(GMPF_Layer*))
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
            filter(lay);

            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    GMPF_saved_state_set_is_saved(flowbox, 0);

    gtk_widget_queue_draw(da);
}


/*
 * Apply the "filter" function to all Layer
 */
void GMPF_filter_apply_to_all_layer(void (*filter)(GMPF_Pixel*))
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
            pixelFilter(lay, filter);

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
                                                              guchar   b)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    {
        D_PRINT("Unable to get layer", NULL);
        return;
    }

    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
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
                                                              guchar   b)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
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
void Lightness(GMPF_Pixel *p)
{
    p->r = p->r > 230 ? 255 : p->r + 25;

    p->g = p->g > 230 ? 255 : p->g + 25;

    p->b = p->b > 230 ? 255 : p->b + 25;

}


/*
 * Apply "Luminosité-" filter to the given Layer
 */
void Darkness(GMPF_Pixel *p)
{

    p->r = p->r < 25 ? 0 : p->r - 25;

    p->g = p->g < 25 ? 0 : p->g - 25;

    p->b = p->b < 25 ? 0 : p->b - 25;
}


/*
 * Apply "Nuance de gris" filter to the given Layer
 */
void Greyscale(GMPF_Pixel *p)
{
    Uint8 grey = (p->r + p->g + p->b) / 3;
    p->r = p->g = p->b = grey;
}


/*
 * Apply "Negatif" filter to the given Layer
 */
void Negative(GMPF_Pixel *p)
{
    p->r = 255 - p->r;
    p->g = 255 - p->g;
    p->b = 255 - p->b;
}


void Binarize (GMPF_Pixel *pixel)
{
    Uint8 grey = (pixel->r + pixel->g + pixel->b) / 3;
    if (grey > 127)
        grey = 255;
    else
        grey = 0;
    pixel->r = pixel->g = pixel->b = grey;
}


/*
 * Apply "Binarisation colorée" filter to the given Layer
 */
void BinarizeColor(GMPF_Pixel *p)
{
    p->r = p->r > 127 ? 255 : 0;

    p->g = p->g > 127 ? 255 : 0;

    p->b = p->b > 127 ? 255 : 0;
}


/*
 * Apply "Teinture" filter to the given Layer
 */
void Tinter(GMPF_Pixel *p)
{
    GET_UI(GtkColorChooser, colorChooser, "ColorTinter");
    guchar r, g, b, factor;
    GdkRGBA rgba;
    gtk_color_chooser_get_rgba (colorChooser, &rgba);
    r = (guchar)(rgba.red * 255);
    g = (guchar)(rgba.green * 255);
    b = (guchar)(rgba.blue * 255);

    factor = 50;

    p->r = p->r * (100 - factor) / 100 + r * factor / 100;
    p->g = p->g * (100 - factor) / 100 + g * factor / 100;
    p->b = p->b * (100 - factor) / 100 + b * factor / 100;
}


/*
 * Apply "Coloré" filter to the given Layer
 */
void Colorfull(GMPF_Pixel *p)
{
    guchar r, g, b, factor;
    GdkRGBA rgba;

    GET_UI(GtkColorChooser, colorChooser, "ColorTinter");
    gtk_color_chooser_get_rgba (colorChooser, &rgba);
    r = (guchar)(rgba.red * 255);
    g = (guchar)(rgba.green * 255);
    b = (guchar)(rgba.blue * 255);
    factor = (guchar)(rgba.alpha * 255);

    p->r = p->r * (100 - factor) / 100 + r * factor / 100;
    p->g = p->g * (100 - factor) / 100 + g * factor / 100;
    p->b = p->b * (100 - factor) / 100 + b * factor / 100;

}


/*
 * Apply "Egalisation" filter to the given Layer
 */
void Equalize(GMPF_Layer *lay)
{
    if (lay->image)
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
    if (lay->image)
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
    if (lay->image)
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
    if (lay->image)
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
    if (lay->image)
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


/*
 * Stucture to store subdivided convolution info
 */
struct ConvoluteThread {
    GdkPixbuf *imgPixbuf;
    GdkPixbuf*img;
    double *mat;
    int mat_size;
    int height;
    int width;
    int width_begin;
    int width_end;
};


/*
 * Function called for subdivided image in convolution
 */
void *subConvolute(void *arg)
{
    struct ConvoluteThread *cvt = (struct ConvoluteThread *) arg;
    gboolean error = FALSE;
    int x = cvt->mat_size;
    double r, g, b, a;
    for(int i = cvt->width_begin; i < cvt->width_end; i++)
    {
        for(int j = 0; j < cvt->height; j++)
        {
            r = g = b = a = 0;
            for (int k = -x / 2; k <= x/2; k++)
            {
                for(int l = -x / 2; l <= x/2; l++)
                {
                    if (check(cvt->width, cvt->height, i + k, j +l))
                    {
                        guchar red, green, blue, alpha;
                        error = gdkpixbuf_get_colors_by_coordinates(cvt->imgPixbuf, i + k, j + l, &red, &green, &blue, &alpha);
                        if(!error)
                        {
                            PRINTERR ("Unable to get pixel");
                            // free_img_rgb(cvt->img);
                            free(cvt->mat);
                            return NULL;
                        }
                        r += cvt->mat[(l + x/2) * x + k + x/2] * (double)red;
                        g += cvt->mat[(l + x/2) * x + k + x/2] * (double)green;
                        b += cvt->mat[(l + x/2) * x + k + x/2] * (double)blue;
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

            put_pixel(cvt->img, i, j, (char)r, (char)g, (char)b, (char)a);
        }
    }
    return NULL;
}


/*
 * Apply the given convolution matrix to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void Convolute(GMPF_Layer *lay, double *mat, size_t mat_size)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    if (lay == NULL)
        return;
    GMPF_saved_state_set_is_saved(flowbox, 0);

    if (lay->image)
        g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    GdkPixbuf *imgPixbuf = lay->image;

    // double r, g, b, a;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    // gboolean error = FALSE;

    int x = mat_size;
    // struct Img_rgb *img = init_img_rgb(width, height);
    GMPF_Size size = {.w=width, .h=height };
    GdkPixbuf *img = new_pixbuf_standardized(&size);

    pthread_t parr[8];
    struct ConvoluteThread carr[8];
    int threadwidth = width >> 3;
    int actualwidth = 0;
    for (int i = 0; i < 7; i++)
    {
        carr[i].imgPixbuf = imgPixbuf;
        carr[i].img = img;
        carr[i].mat = mat;
        carr[i].mat_size = x;
        carr[i].height = height;
        carr[i].width_begin = actualwidth;
        carr[i].width = width;
        actualwidth += threadwidth;
        carr[i].width_end = actualwidth;
        int e = pthread_create(&parr[i], NULL, subConvolute, &carr[i]);
        if (e)
        { PRINTERR("Unable to create thread"); }
    }
    carr[7].imgPixbuf = imgPixbuf;
    carr[7].img = img;
    carr[7].mat = mat;
    carr[7].mat_size = x;
    carr[7].height = height;
    carr[7].width_begin = actualwidth;
    carr[7].width = width;
    actualwidth += threadwidth;
    carr[7].width_end = width;
    int e = pthread_create(&parr[7], NULL, subConvolute, &carr[7]);
    if (e)
    { PRINTERR("Unable to create thread"); }

    for (int i = 0; i < 8; i++)
    {
        pthread_join(parr[i], NULL);
    }

    g_object_unref(imgPixbuf);
    lay->image = img;
    while (cairo_surface_get_reference_count(lay->surface))
        cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(img, 1, NULL);
    layer_icon_refresh(lay);
    gtk_widget_queue_draw(da);
}




/*
 * Stucture to store subdivided convolution info
 */
struct PixelFilterThread {
    GdkPixbuf *imgPixbuf;
    void (*filter)(GMPF_Pixel*);
    int height;
    int width;
    int width_begin;
    int width_end;
};


/*
 * Function called for subdivided image in convolution
 */
void *subPixelFilter(void *arg)
{
    struct PixelFilterThread *cvt = (struct PixelFilterThread *) arg;
    GMPF_Pixel *pixel = (GMPF_Pixel *) gdk_pixbuf_get_pixels(cvt->imgPixbuf);

    for(int i = cvt->width_begin; i < cvt->width_end; i++)
    {
        for(int j = 0; j < cvt->height; j++)
        {
            cvt->filter(pixel + i + j*cvt->width);
        }
    }
    return NULL;
}


/*
 * Apply the given convolution matrix to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void pixelFilter(GMPF_Layer *lay, void (*filter)(GMPF_Pixel*))
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    if (lay == NULL)
        return;
    GMPF_saved_state_set_is_saved(flowbox, 0);

    if (lay->image)
        g_object_unref(lay->image);
    lay->image = gdk_pixbuf_get_from_surface(lay->surface, 0, 0, lay->size.w, lay->size.h);

    int width = gdk_pixbuf_get_width(lay->image);
    int height = gdk_pixbuf_get_height(lay->image);

    pthread_t parr[8];
    struct PixelFilterThread carr[8];
    int threadwidth = width >> 3;
    int actualwidth = 0;
    for (int i = 0; i < 7; i++)
    {
        carr[i].imgPixbuf = lay->image;
        carr[i].filter = filter;
        carr[i].height = height;
        carr[i].width_begin = actualwidth;
        carr[i].width = width;
        actualwidth += threadwidth;
        carr[i].width_end = actualwidth;
        int e = pthread_create(&parr[i], NULL, subPixelFilter, &carr[i]);
        if (e)
        { PRINTERR("Unable to create thread"); }
    }
    carr[7].imgPixbuf = lay->image;
    carr[7].filter = filter;
    carr[7].height = height;
    carr[7].width_begin = actualwidth;
    carr[7].width = width;
    carr[7].width_end = width;
    int e = pthread_create(&parr[7], NULL, subPixelFilter, &carr[7]);
    if (e)
    { PRINTERR("Unable to create thread"); }

    for (int i = 0; i < 8; i++)
    {
        pthread_join(parr[i], NULL);
    }

    // g_object_unref(imgPixbuf);
    // lay->image = img;
    while (cairo_surface_get_reference_count(lay->surface))
        cairo_surface_destroy(lay->surface);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 1, NULL);
    layer_icon_refresh(lay);
    gtk_widget_queue_draw(da);
}
