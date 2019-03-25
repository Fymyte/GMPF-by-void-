#include "callback.h"

struct _GdkPixbuf *unchangedPixbuf;

#define GET_UI(_type, _name) \
    (_type *) (gtk_builder_get_object(data->builder, _name));

#define D_PRINT(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "debug: %s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

/* change the cursor
values :
0 ==> normal
1 ==> draw_brush
2 ==> rubber
*/

/*
Use this to convert our imga with multiple layer in one pixbuf
GdkPixbuf *
gdk_pixbuf_get_from_drawable (GdkPixbuf *dest,
                              GdkDrawable *src,
                              GdkColormap *cmap,
                              int src_x,
                              int src_y,
                              int dest_x,
                              int dest_y,
                              int width,
                              int height);
                              */
int cursor_state = 0;

int check(int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i > width || j > height)
    return 0;
    return 1;
}

void callback_flip(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkImage *image = NULL;

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));
    GdkPixbuf *pixbuf;

    const char *menulabel = gtk_menu_item_get_label (menuitem);
    D_PRINT("%s\n", menulabel);

    if (strcmp(menulabel, "Flip horizontal"))
    pixbuf = gdk_pixbuf_flip (unchangedPixbuf, TRUE);

    else
    pixbuf = gdk_pixbuf_flip (unchangedPixbuf, FALSE);

    g_object_unref(unchangedPixbuf);
    unchangedPixbuf = pixbuf;
    gtk_image_set_from_pixbuf(image, pixbuf);
}

void callback_layer_set_visible(GtkToggleButton *button, gpointer user_data)
{
    SGlobalData *data = (SGlobalData *)user_data;
    GtkFlowBox *flowbox = GET_UI(GtkFlowBox, "GMPF_flowbox");

    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    if (! layer)
        D_PRINT("Unable to get selected layer", NULL);
    else
        layer->isvisible = gtk_toggle_button_get_active(button);
}

void callback_rotate(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkImage *image = NULL;

    image = GET_UI(GtkImage, "OriginalImage");

    // image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));
    GdkPixbuf *pixbuf;

    const char *menulabel = gtk_menu_item_get_label (menuitem);
    D_PRINT("%s\n", menulabel);
    if (strcmp(menulabel, "Rotate left"))
    {
        pixbuf = gdk_pixbuf_rotate_simple(unchangedPixbuf, GDK_PIXBUF_ROTATE_CLOCKWISE);
    }
    else
    {
        pixbuf = gdk_pixbuf_rotate_simple(unchangedPixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
    }

    g_object_unref(unchangedPixbuf);
    unchangedPixbuf = pixbuf;
    gtk_image_set_from_pixbuf(image, pixbuf);
}


void callback_hideWidget(GtkWidget *widget, gpointer user_data)
{
    gtk_widget_hide(widget);
    user_data = 0;
}


void callback_about (GtkMenuItem *menuitem, gpointer user_data)
{
    /* Transtypage du pointeur user_data pour récupérer nos données. */
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *dialog = NULL;

    dialog =  gtk_about_dialog_new ();

    /* Pour l'exemple on va rendre la fenêtre "À propos" modale par rapport à la */
    /* fenêtre principale. */
    gtk_window_set_transient_for (GTK_WINDOW(dialog),
    GTK_WINDOW(gtk_builder_get_object (data->builder, "MainWindow")));

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    menuitem = 0;
}


void callback_adjust_scale(GtkEntry *entry, gpointer user_data)
{
    const gchar *s = gtk_entry_get_text (entry);
    float scaleValue = atof(s) / 100;
    adjust_scale (scaleValue, scaleValue, user_data);
}

void callback_resize_brush(GtkEntry *entry, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GMPF_LayerMngr *layermngr = NULL;
    GtkFlowBox *flowbox = NULL;
    flowbox = GET_UI(GtkFlowBox, "GMPF_flowbox");
    layermngr = layermngr_get_layermngr(flowbox);

    const gchar *s = gtk_entry_get_text (entry);
    float size = atof(s);
    layermngr->brush_size = size;
    // resizeCursor(data, (int)size);
}

void callback_save(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    export_cairo_to_png(data);
    (void)menuitem;
}

void adjust_scale(double scale_x, double scale_y, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkWidget *da = NULL;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);


    D_PRINT("w: %d, h: %d\n", layermngr->size.w, layermngr->size.h);

    double max_width = 0;
    double max_height = 0;

    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {

            lay->scale_factor.x = scale_x;
            lay->scale_factor.y = scale_y;

            if (lay->size.w * scale_x > max_width)
                max_width = lay->size.w * scale_x;
            if (lay->size.h * scale_y > max_height)
                max_height = lay->size.h * scale_y;

            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_widget_queue_draw(da);
}


void clear_surface (gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Layer *lay;
    if((lay = layermngr_get_selected_layer(flowbox)) != NULL)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    cairo_t *cr;

    cr = cairo_create (layermngr->surface);

    cairo_set_source_rgba(cr, 0, 1, 0, 0);
    cairo_paint (cr);

    cairo_destroy (cr);

}

    /* Create a new surface of the appropriate size to store our scribbles */
gboolean configure_event_cb (GtkWidget *widget,
    GdkEventConfigure *event,
    gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    if (!flowbox)
        D_PRINT("unable to get flowbox", NULL);

    layermngr = layermngr_get_layermngr(flowbox);
    if (!layermngr)
        D_PRINT("unable to get layermngr", NULL);


    GMPF_Layer *lay;
    if((lay = layermngr_get_selected_layer(flowbox)) != NULL)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    if (layermngr->surface)
        cairo_surface_destroy (layermngr->surface);

    cairo_content_t content =
        cairo_surface_get_content (cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1));

    layermngr->surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
    content,
    gtk_widget_get_allocated_width (widget),
    gtk_widget_get_allocated_height (widget));

    /* Initialize the surface to white */
    clear_surface (user_data);
    (void)event;

    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}


/* Draw a rectangle on the surface at the given position */
void draw_brush (GtkWidget *widget, gdouble x, gdouble y, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GtkColorChooser *chooser = NULL;
    GdkRGBA color;
    GMPF_LayerMngr *layermngr = NULL;


    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    chooser = (GtkColorChooser *)(gtk_builder_get_object(data->builder, "ColorTinter"));
    layermngr = layermngr_get_layermngr(flowbox);
    gtk_color_chooser_get_rgba(chooser, &color);

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay != NULL) // NEW VERSION - AVAILABLE NOW
    {
        /* Paint to the surface, where we store our state */

        lay->cr = cairo_create (lay->surface);

        //begin brush zone
        circular_brush(widget, lay->cr, x, y, layermngr->brush_size,
            (float)color.red, (float)color.green, (float)color.blue,
            (float)color.alpha, lay->scale_factor.x, lay->scale_factor.y,
            layermngr);
        //end brush zone

        cairo_destroy(lay->cr);

    }

    /* Now invalidate the affected region of the drawing area. */


}


void draw_rubber (GtkWidget *widget, gdouble x, gdouble y, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;


    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay != NULL) // NEW VERSION - AVAILABLE NOW
    {

        /* Paint to the surface, where we store our state */

        lay->cr = cairo_create (lay->surface);

        //begin brush zone
        circular_brush(widget, lay->cr, x, y, layermngr->brush_size, 0, 0, 0, 0.0,
                    lay->scale_factor.x, lay->scale_factor.y, layermngr);
        //end brush zone
        cairo_destroy(lay->cr);
    }
}

/* Handle button press events by either drawing a rectangle
* or clearing the surface, depending on which button was pressed.
* The ::button-press signal handler receives a GdkEventButton
* struct which contains this information.
*/

gboolean button_release_event_cb(GtkWidget     *widget,
GdkEventButton *event,
gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);

    layermngr->pos.x = -1;
    layermngr->pos.y = -1;

    widget = 0;
    event = 0;

    return TRUE;
}

gboolean button_press_event_cb (GtkWidget      *widget,
GdkEventButton *event,
gpointer        user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Layer *lay;
    if((lay = layermngr_get_selected_layer(flowbox)) != NULL)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    /* paranoia check, in case we haven't gotten a configure event */
    if (layermngr->surface == NULL)
        return FALSE;

    // if (lay->cr)
    //     cairo_destroy(lay->cr);
    // lay->cr = cairo_create(lay->surface);
    // // cairo_save (lay->cr);

    // layermngr->pos.x = event->x;
    // layermngr->pos.y = event->y;

    if (event->button == GDK_BUTTON_PRIMARY & cursor_state == 1)
        draw_brush (widget, event->x, event->y, user_data);

    else if (event->button == GDK_BUTTON_PRIMARY & cursor_state == 2)
        draw_rubber (widget, event->x, event->y, user_data);

    else if (event->button == GDK_BUTTON_SECONDARY)
    {
        clear_surface (user_data);
        gtk_widget_queue_draw (widget);
    }

    /* We've handled the event, stop processing */
    return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
* still held down. The ::motion-notify signal handler receives
* a GdkEventMotion struct which contains this information.
*/
gboolean
motion_notify_event_cb (GtkWidget *widget, GdkEventMotion *event,
    gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Layer *lay;

    if((lay = layermngr_get_selected_layer(flowbox)) != NULL)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    /* paranoia check, in case we haven't gotten a configure event */
    if (layermngr->surface == NULL)
        return FALSE;

    if (cursor_state == 1 && (event->state & GDK_BUTTON1_MASK))
        draw_brush (widget, event->x, event->y, user_data);
    if (cursor_state == 2 && (event->state & GDK_BUTTON1_MASK))
        draw_rubber (widget, event->x, event->y, user_data);


    /* We've handled it, stop processing */
    return TRUE;
}


void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    flowbox = (GtkFlowBox *)(gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);

    cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);

    // int cur_lay = 0; // Use this variable for debuging

    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {

            // D_PRINT("drawing layer %i\n", cur_lay);
            if (lay->isvisible)
            {
                cairo_save(cr);
                // cairo_surface_t *surface = lay->surface ? lay->surface : lay->surface;
                cairo_scale(cr, lay->scale_factor.x, lay->scale_factor.y);
                cairo_set_source_surface (cr, lay->surface, (double)lay->pos.x, (double)lay->pos.y);
                cairo_paint(cr);
                cairo_restore(cr);
            }
            lay->image = gdk_pixbuf_get_from_surface (lay->surface, lay->pos.x, lay->pos.y,
                                                                        lay->size.w, lay->size.h);
            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    // cairo_destroy(cr);
    (void)user_data;
    (void)widget;
}


void callback_image_cairo(GtkFileChooser *btn, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    GError *error = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    gchar *filename = gtk_file_chooser_get_filename(btn);


    GtkFlowBox *flowbox = NULL;
    flowbox = GET_UI(GtkFlowBox, "GMPF_flowbox");
    // flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);
    layermngr_add_new_layer(flowbox, filename);

    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
    if(da == NULL)
        D_PRINT("unable to get the drawingArea", NULL);

    // layout = GTK_WIDGET(gtk_builder_get_object(data->builder, "Layout"));
    int width, height;
    int max_width  = layermngr->size.w;
    int max_height = layermngr->size.h;

    layermngr->image  = gdk_pixbuf_new_from_file(filename, &error);
    width  = gdk_pixbuf_get_width  (layermngr->image);
    height = gdk_pixbuf_get_height (layermngr->image);

    if (width > max_width)
        max_width = width;
    if (height > max_height)
        max_height = height;

    layermngr->size.w = max_width;
    layermngr->size.h = max_height;

    layermngr->surface = gdk_cairo_surface_create_from_pixbuf(layermngr->image, 0, NULL);

    gtk_widget_set_size_request(da, max_width, max_height);

    D_PRINT("max_width: %d, max_height = %d\n", max_width, max_height);

    if(error)
    {
        printf("Error : %s\n", error->message);
        g_error_free(error);
    }

    if (!gdk_pixbuf_get_has_alpha(layermngr->image))
    {
        GdkPixbuf *i = gdk_pixbuf_add_alpha ((const GdkPixbuf *)layermngr->image, FALSE, 0, 0, 0);
        g_object_unref (layermngr->image);
        layermngr->image = i;
    }

    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), user_data);
}


void callback_binarize(GtkMenuItem *menuitem, gpointer user_data)
// IS OK
{
    menuitem = 0;
    g_print("Binarize\n");

    SGlobalData *data = (SGlobalData*) user_data;

    GtkFlowBox *flowbox =
        (GtkFlowBox*)(gtk_builder_get_object(data -> builder, "GMPF_flowbox"));

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

    int width = (lay -> size).w;
    int height = (lay -> size).h;

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


void callback_binarize_color(GtkMenuItem *menuitem, gpointer user_data)
// IS OK
{
    menuitem = 0;
    g_print("Binarize color\n");

    SGlobalData *data = (SGlobalData*) user_data;

    GtkFlowBox *flowbox =
        (GtkFlowBox*)(gtk_builder_get_object(data -> builder, "GMPF_flowbox"));

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

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
    cairo_destroy(cr);
    free(pos);
    free(pixel);
}

/*
void callback_convolute_f(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Convolution\n");
    int number;
    const char *menulabel = gtk_menu_item_get_label (menuitem);
    if (!strcmp(menulabel, "Bords"))
    {
        number = 0;
    }
    else if (!strcmp(menulabel, "Epine"))
    {
        number = 1;
    }
    else
    {
        number = 2;
    }
    // printf("Waiting for a number....\n");
    // printf("0 : Edge Enhance\n");
    // printf("1 : Sharpen\n");
    // printf("2 : Blur\n");
    // if (scanf("%i", &number) == EOF)
    //     errx(1, "Error, scanf()");


    double *mat = malloc(sizeof(double) * 9);
    switch (number)
    {
        case 0 :
        printf("Edge Enhance\n");
        mat[0] = mat[1] = mat[2] = mat[3] = mat[5] = mat[6] = mat[7] = mat[8] = -1;
        mat[4] = 8;
        break;
        case 1 :
        printf("Sharpen\n");
        mat[0] = mat[2] = mat[6] = mat[8] = 0;
        mat[1] = mat[3] = mat[5] = mat[7] = -1;
        mat[4] = 5;
        break;
        default :
        printf("Blur\n");
        mat[0] = mat[1] = mat[2] = mat[3] = mat[4] = mat[5] = mat[6] = mat[7] = mat[8] = (double) 1/9;
        break;
    }

    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
                        error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
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
    unchangedPixbuf = imgPixbuf;
    gtk_image_set_from_pixbuf(image, imgPixbuf);

    free_img_rgb(img);
    free(mat);
}
*/

void callback_grey(GtkMenuItem *menuitem, gpointer user_data)
// NOT OK
{
    menuitem = 0;
    g_print("Grayscale\n");

    SGlobalData *data = (SGlobalData*) user_data;

    GtkFlowBox *flowbox =
        (GtkFlowBox*)(gtk_builder_get_object(data -> builder, "GMPF_flowbox"));

    struct GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
        return;

    int width = (lay -> size).w;
    int height = (lay -> size).h;

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

            cairo_set_source_rgba(cr, grey, grey, grey, pixel->A);
            cairo_move_to(cr, i, j);
            cairo_rel_line_to(cr, 0, 1);
            cairo_stroke(cr);
        }
    }
    //lay -> cr = cr;
    cairo_destroy(cr);
    free(pos);
    free(pixel);
}


void callback_brush(GtkMenuItem *menuitem, gpointer user_data)
{
    (void)menuitem;
    cursor_state = 1;
    SGlobalData *data = (SGlobalData*) user_data;
    callback_setCursor(data);
}

void callback_rubber(GtkMenuItem *menuitem, gpointer user_data)
{
    (void)menuitem;
    cursor_state = 2;
    SGlobalData *data = (SGlobalData*) user_data;
    callback_setCursor(data);
}

void callback_FC(GtkMenuItem *menuitem, gpointer user_data)
{
    //variables definitions
    SGlobalData *data = (SGlobalData*)user_data;
    GtkWidget *FCWindow = NULL;
    GtkImage *test_image = NULL;
    GError *err = NULL;
    struct _GdkPixbuf *imgPixbuf = NULL;
    imgPixbuf = gdk_pixbuf_new_from_file(
    "image_test.jpg", &err);

    if(err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
    }

    FCWindow = GTK_WIDGET(gtk_builder_get_object(data->builder, "FilterCreator"));
    test_image = GTK_IMAGE(gtk_builder_get_object(data->builder, "Image_test"));

    //test image resize + setting
    int pixbuf_width = gdk_pixbuf_get_width(imgPixbuf) / 2;
    int pixbuf_height = gdk_pixbuf_get_height(imgPixbuf) / 2;


    struct _GdkPixbuf *img2 = gdk_pixbuf_scale_simple(imgPixbuf,
    pixbuf_width, pixbuf_height, GDK_INTERP_BILINEAR);

    gtk_image_clear(test_image);
    gtk_image_set_from_pixbuf(test_image, img2);

    //show the filter creator windowjust
    gtk_widget_show(FCWindow);
    menuitem = 0;
}

/*
void callback_vertical(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Mirror - Vertical\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    struct Img_rgb *img = init_img_rgb(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            guchar red, green, blue, alpha;
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
            if(!error)
            err(1, "pixbuf get pixels error");
            Matrix_val(img -> red, width - i - 1, j, (double)red);
            Matrix_val(img -> green, width - i - 1, j, (double)green);
            Matrix_val(img -> blue, width - i - 1, j, (double)blue);
            Matrix_val(img -> alpha, width - i - 1, j, (double)alpha);
        }
    }
    Img_rgb_to_Image(imgPixbuf, img);
    gtk_image_set_from_pixbuf(image, imgPixbuf);
    free_img_rgb(img);
}


void callback_tinter(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Tinter\n");
    menuitem = 0;

    guchar r, g, b, factor;

    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    GtkColorChooser *colorChooser = NULL;
    GdkRGBA rgba;
    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
    colorChooser = (GtkColorChooser *)(gtk_builder_get_object(data->builder, "ColorTinter"));
    gtk_color_chooser_get_rgba (colorChooser, &rgba);
    r = (guchar)(rgba.red * 255);
    g = (guchar)(rgba.green * 255);
    b = (guchar)(rgba.blue * 255);
    factor = (guchar)(rgba.alpha * 100);

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
    layermngr->surface = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
}


void callback_colorfull(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Colorfull\n");
    menuitem = 0;
    guchar r, g, b, factor;

    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    GtkColorChooser *colorChooser = NULL;
    GdkRGBA rgba;
    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
    colorChooser = (GtkColorChooser *)(gtk_builder_get_object(data->builder, "ColorTinter"));
    gtk_color_chooser_get_rgba (colorChooser, &rgba);
    r = (guchar)(rgba.red * 255);
    g = (guchar)(rgba.green * 255);
    b = (guchar)(rgba.blue * 255);
    factor = (guchar)(rgba.alpha * 255);

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
    layermngr->surface = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
}


void callback_negative(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Negative\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
    layermngr->surface = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
}


void callback_horizontal(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Mirror - Horizontal\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    struct Img_rgb *img = init_img_rgb(width, height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            guchar red, green, blue, alpha;
            error= gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
            if(!error)
            err(1, "pixbuf get pixels error");
            Matrix_val(img -> red, i, height - j - 1, (double)red);
            Matrix_val(img -> green, i , height - j - 1, (double)green);
            Matrix_val(img -> blue, i, height - j - 1, (double)blue);
            Matrix_val(img -> alpha, i, height - j - 1, (double)alpha);
        }
    }
    Img_rgb_to_Image(imgPixbuf, img);
    gtk_image_set_from_pixbuf(image, imgPixbuf);
    free_img_rgb(img);
}
*/
void reset_cursor(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    resetCursor(data);
    cursor_state = 0; // 0 ==> normal cursor
    menuitem = 0;
}
