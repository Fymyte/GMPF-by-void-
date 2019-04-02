#include "callback.h"

// struct _GdkPixbuf *unchangedPixbuf;

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

/*int check(int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i > width || j > height)
        return 0;
    return 1;
}*/
void callback_rotate_angle(GtkEntry *entry, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return;

    lay->rotate_angle = atoi(gtk_entry_get_text(entry));
    gtk_widget_queue_draw(da);
}

void callback_rotate_angle_all(GtkEntry *entry, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    const gchar *s = gtk_entry_get_text (entry);
    layer_rotate_angle_all (atoi(s), layermngr_get_layermngr(flowbox));

    gtk_widget_queue_draw(da);
}

void layer_rotate_angle_all(int angle, GMPF_LayerMngr *layermngr)
{
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            lay->rotate_angle = angle;

            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
}


void callback_flip(UNUSED GtkMenuItem *menuitem, UNUSED gpointer user_data)
{
    // INIT_UI();
    //
    // GtkImage *image = NULL;
    //
    // image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));
    // GdkPixbuf *pixbuf;
    //
    // const char *menulabel = gtk_menu_item_get_label (menuitem);
    //
    // if (strcmp(menulabel, "Flip horizontal"))
    // pixbuf = gdk_pixbuf_flip (unchangedPixbuf, TRUE);
    //
    // else
    // pixbuf = gdk_pixbuf_flip (unchangedPixbuf, FALSE);
    //
    // g_object_unref(unchangedPixbuf);
    // unchangedPixbuf = pixbuf;
    // gtk_image_set_from_pixbuf(image, pixbuf);
}

void callback_layer_set_visible(GtkToggleButton *button, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    if (layer)
    {
        layer->isvisible = gtk_toggle_button_get_active(button);
        return;
    }
    D_PRINT("Unable to get selected layer", NULL);
}

void callback_layer_move_down(UNUSED GtkWidget *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData *)user_data;
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    layermngr_move_down_selected_layer(flowbox);
    gtk_widget_queue_draw(da);
}

void callback_layer_move_up(UNUSED GtkWidget *widget, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    layermngr_move_up_selected_layer(flowbox);
    gtk_widget_queue_draw(da);;
}

// void callback_rotate(GtkMenuItem *menuitem, gpointer user_data)
// {
//     INIT_UI();
//
//     GET_UI(GtkImage, image, "OriginalImage");
//
//     // image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));
//     GdkPixbuf *pixbuf;
//
//     const char *menulabel = gtk_menu_item_get_label (menuitem);
//     D_PRINT("%s", menulabel);
//     if (strcmp(menulabel, "Rotate left"))
//     {
//         pixbuf = gdk_pixbuf_rotate_simple(unchangedPixbuf, GDK_PIXBUF_ROTATE_CLOCKWISE);
//     }
//     else
//     {
//         pixbuf = gdk_pixbuf_rotate_simple(unchangedPixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
//     }
//
//     g_object_unref(unchangedPixbuf);
//     unchangedPixbuf = pixbuf;
//     gtk_image_set_from_pixbuf(image, pixbuf);
// }

void callback_hideWidget(GtkWidget *widget, UNUSED gpointer user_data)
{
    gtk_widget_hide(widget);
}

void callback_hideParent(GtkWidget *widget, GtkWidget *parent)
{
    gtk_widget_hide(parent);
}

void callback_about (UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    /* Transtypage du pointeur user_data pour récupérer nos données. */
    INIT_UI();
    GET_UI(GtkWidget, dialog, "AboutWindow");

    /* Pour l'exemple on va rendre la fenêtre "À propos" modale par rapport à la */
    /* fenêtre principale. */
    GET_UI(GtkWindow, window, "MainWindow");
    gtk_window_set_transient_for (GTK_WINDOW(dialog), window);

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void callback_adjust_scale(GtkEntry *entry, gpointer user_data)
{
    const gchar *s = gtk_entry_get_text (entry);
    float scaleValue = atof(s) / 100;
    adjust_scale (scaleValue, scaleValue, user_data);
}

void callback_showDialog(UNUSED GtkMenuItem *menuitem, GtkWidget *dialog)
{
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void callback_save(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    export_cairo_to_png(data);
}

void callback_resize_brush(GtkEntry *entry, gpointer user_data)
{
    INIT_UI();
    GMPF_LayerMngr *layermngr = NULL;
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    layermngr = layermngr_get_layermngr(flowbox);

    const gchar *s = gtk_entry_get_text (entry);
    float size = atof(s);
    layermngr->brush_size = size;
    // resizeCursor(data, (int)size);
}

void callback_show_layer_window(UNUSED GtkWidget *widget, gpointer user_data)
{
    //variables definitions
    INIT_UI();
    GET_UI(GtkWidget, layer_window, "LayerWindow");
    GET_UI(GtkEntry, name, "LayerNameEntry");
    GET_UI(GtkSpinButton, width, "LayerWidthSpinButton");
    GET_UI(GtkSpinButton, height, "LayerHeightSpinButton");
    GET_UI(GtkSpinButton, offsetX, "LayerOffsetXSpinButton");
    GET_UI(GtkSpinButton, offsetY, "LayerOffsetYSpinButton");
    GET_UI(GtkFileChooser, filename, "LayerImageFilename");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    gtk_entry_set_text(name, "");
    gtk_spin_button_set_value(width, layermngr->size.w);
    gtk_spin_button_set_value(height, layermngr->size.h);
    gtk_spin_button_set_value(offsetX, 0);
    gtk_spin_button_set_value(offsetY, 0);
    gtk_file_chooser_set_filename(filename, "gimp_log.png");

    gtk_widget_show(layer_window);
}

void adjust_scale(double scale_x, double scale_y, gpointer user_data)
{
    INIT_UI();


    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    //GMPF_Layer *selected_layer = layermngr_get_selected_layer(flowbox);

    double max_width = 0;
    double max_height = 0;

    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            // if (lay == selected_layer)
            //     cairo_surface_reference(lay->surface);

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
    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
    gtk_widget_queue_draw(da);
}

void clear_surface (gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if(lay)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    cairo_t *cr = cairo_create (layermngr->surface);

    cairo_set_source_rgba(cr, 0, 1, 0, 0);
    cairo_paint (cr);

    cairo_destroy (cr);
}

    /* Create a new surface of the appropriate size to store our scribbles */
gboolean configure_event_cb (GtkWidget *widget,
    UNUSED GdkEventConfigure *event,
    gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (!layermngr)
        D_PRINT("unable to get layermngr", NULL);

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if((lay) != NULL)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    cairo_content_t content =
        cairo_surface_get_content (cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1));

    layermngr->surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
    content,
    gtk_widget_get_allocated_width (widget),
    gtk_widget_get_allocated_height (widget));
    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}

/* Draw a rectangle on the surface at the given position */
void draw_brush (GtkWidget *widget, gdouble x, gdouble y, gpointer user_data)
{
    INIT_UI();
    GdkRGBA color;

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkColorChooser, chooser, "ColorTinter");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    gtk_color_chooser_get_rgba(chooser, &color);

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay != NULL) // NEW VERSION - AVAILABLE NOW
    {
        /* Paint to the surface, where we store our state */

        lay->cr = cairo_create (lay->surface);

        //begin brush zone
        circular_brush(widget, lay->cr, x - lay->pos.x, y - lay->pos.y,lay->pos.x, lay->pos.y, layermngr->brush_size,
            (float)color.red, (float)color.green, (float)color.blue,
            (float)color.alpha, lay->scale_factor.x, lay->scale_factor.y,
            layermngr);
        //end brush zone

        cairo_destroy(lay->cr);
    }
}

void draw_rubber (GtkWidget *widget, gdouble x, gdouble y, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay != NULL) // NEW VERSION - AVAILABLE NOW
    {
        /* Paint to the surface, where we store our state */
        lay->cr = cairo_create (lay->surface);

        //begin brush zone
        circular_brush(widget, lay->cr, x + lay->pos.x, y + lay->pos.y,lay->pos.x, lay->pos.y, layermngr->brush_size, 0, 0, 0, 0.0,
                    lay->scale_factor.x, lay->scale_factor.y, layermngr);
        //end brush zone
        cairo_destroy(lay->cr);
    }
}

gboolean enter_notify_event_cb (UNUSED GtkWidget *widget,
                        UNUSED GdkEvent *event, gpointer user_data)
{
    INIT_UI();
    callback_setCursor(data);
    return TRUE;
}

gboolean leave_notify_event_cb (UNUSED GtkWidget *widget,
                        UNUSED GdkEvent *event, gpointer user_data)
{
    INIT_UI();
    resetCursor(data);
    return TRUE;
}
/* Handle button press events by either drawing a rectangle
* or clearing the surface, depending on which button was pressed.
* The ::button-press signal handler receives a GdkEventButton
* struct which contains this information.
*/

gboolean button_release_event_cb(UNUSED GtkWidget *widget,
                        UNUSED GdkEventButton *event, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    layermngr->pos.x = -1;
    layermngr->pos.y = -1;

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return FALSE;

    REFRESH_IMAGE(lay);

    return TRUE;
}

gboolean button_press_event_cb (GtkWidget      *widget,
GdkEventButton *event,
gpointer        user_data)
{
    INIT_UI();

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Layer *lay =layermngr_get_selected_layer(flowbox);
    if(lay)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    /* paranoia check, in case we haven't gotten a configure event */
    if (layermngr->surface == NULL)
        return FALSE;

    GMPF_Tool tool = layermngr->tool;

    if (event->button == GDK_BUTTON_PRIMARY & tool == PAINTER)
        draw_brush (widget, event->x, event->y, user_data);

    else if (event->button == GDK_BUTTON_PRIMARY & tool == ERAISER)
        draw_rubber (widget, event->x, event->y, user_data);

    // else if (event->button == GDK_BUTTON_SECONDARY)
    // {
    //     clear_surface (user_data);
    //     gtk_widget_queue_draw (widget);
    // }

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
    INIT_UI();

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if(lay)
        layermngr->surface = lay->surface;
    else
        layermngr->surface = NULL;

    /* paranoia check, in case we haven't gotten a configure event */
    if (layermngr->surface == NULL)
        return FALSE;

    GMPF_Tool tool = layermngr->tool;

    if (tool == PAINTER && (event->state & GDK_BUTTON1_MASK))
        draw_brush (widget, event->x, event->y, user_data);
    if (tool == ERAISER && (event->state & GDK_BUTTON1_MASK))
        draw_rubber (widget, event->x, event->y, user_data);


    /* We've handled it, stop processing */
    return TRUE;
}


void on_draw_event(UNUSED GtkWidget *widget, cairo_t *cr, UNUSED gpointer user_data)
{
    INIT_UI();

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);

    int cur_lay = 0; // Use this variable for debuging
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            cur_lay++;
            if (lay->isvisible)
            {
                cairo_save(cr);
                cairo_scale(cr, lay->scale_factor.x, lay->scale_factor.y);
                // double tx = lay->rotate_angle < 90 ? lay->rotate_angle * lay->size.w / 90 : lay->size.w - (lay->rotate_angle * lay->size.w / 90);
                // double ty = lay->rotate_angle < 90 ? 0 : lay->size.h - (lay->rotate_angle * lay->size.h / 90);;
                cairo_translate(cr, lay->size.w / 2, lay->size.h / 2);
                cairo_rotate(cr, RAD_FROM_DEG(lay->rotate_angle));
                cairo_translate(cr, -lay->size.w / 2, -lay->size.h / 2);
                cairo_set_source_surface (cr, lay->surface, (double)lay->pos.x, (double)lay->pos.y);
                cairo_paint(cr);
                cairo_restore(cr);
            }
            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
}

void callback_select_tool(GtkWidget *widget, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    gchar name = gtk_widget_get_name(widget)[0];
    GMPF_Tool tool;
    switch (name)
    {
        case '1': tool = PAINTER;
                         break;
        case '2': tool = ERAISER;
                         break;
        case '3': tool = COLOR_PICKER;
                         break;
        case '4': tool = SELECTOR;
                         break;
        case '5': tool = SELECTOR_FREE;
                         break;
        default : tool = INCORECT_TOOL;
        D_PRINT("Unknown tool", NULL);
    }
    layermngr->tool = tool;
}

void callback_add_custom_layer(UNUSED GtkWidget *widget, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkEntry, name, "LayerNameEntry");
    GET_UI(GtkEntry, width, "LayerWidthSpinButton");
    GET_UI(GtkEntry, height, "LayerHeightSpinButton");
    GET_UI(GtkEntry, offsetX, "LayerOffsetXSpinButton");
    GET_UI(GtkEntry, offsetY, "LayerOffsetYSpinButton");
    GET_UI(GtkFileChooser, filename, "LayerImageFilename");
    GET_UI(GtkWidget, window, "LayerWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    const gchar *n = gtk_entry_get_text(name);
    const gchar *w = gtk_entry_get_text(width);
    const gchar *h = gtk_entry_get_text(height);
    const gchar *x = gtk_entry_get_text(offsetX);
    const gchar *y = gtk_entry_get_text(offsetY);
    const gchar *f = gtk_file_chooser_get_filename(filename);

    GMPF_Layer *lay = layermngr_add_new_layer(flowbox, f);
    lay->name = (char*)n;
    lay->size.w = atoi(w);
    lay->size.h = atoi(h);
    lay->pos.x = atoi(x);
    lay->pos.y = atoi(y);

    gtk_widget_hide(window);
}

void callback_save_project(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");

    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                          window,
                                          action,
                                          ("_Cancel"),
                                          GTK_RESPONSE_CANCEL,
                                          ("_Save"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    chooser = GTK_FILE_CHOOSER (dialog);

    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_current_name (chooser,
                                         ("Untitled.gmpf"));

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (chooser);
        char err = save_project(flowbox, (const char*)filename);
        if (err)
        {
            D_PRINT("Unable to save project", NULL);
        }
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

void callback_load_project(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          window,
                                          action,
                                          ("_Cancel"),
                                          GTK_RESPONSE_CANCEL,
                                          ("_Open"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        char err = load_project (flowbox, filename);
        if (err)
            D_PRINT("Uable to load project", NULL);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);

}

void callback_image_cairo(GtkFileChooser *btn, gpointer user_data)
{
    INIT_UI();
    GError *error = NULL;

    gchar *filename = gtk_file_chooser_get_filename(btn);

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr_add_new_layer(flowbox, filename);

    GET_UI(GtkWidget, da, "drawingArea");
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
    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);

    D_PRINT("max_width: %d, max_height = %d", max_width, max_height);

    if(error)
    {
        printf("Error : %s", error->message);
        g_error_free(error);
    }

    if (!gdk_pixbuf_get_has_alpha(layermngr->image))
    {
        GdkPixbuf *i = gdk_pixbuf_add_alpha ((const GdkPixbuf *)layermngr->image, FALSE, 0, 0, 0);
        g_object_unref (layermngr->image);
        layermngr->image = i;
    }
}

void callback_binarize(UNUSED GtkMenuItem *menuitem, gpointer user_data)
// IS OK
{
    INIT_UI();
    Binarize(data);
}


void callback_binarize_color(UNUSED GtkMenuItem *menuitem, gpointer user_data)
// IS OK
{
    INIT_UI();
    Equalize(data);
}


void callback_convolute_f(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    D_PRINT("Convolution", NULL);
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
    Convolute(data, mat);
}


void callback_grey(UNUSED GtkMenuItem *menuitem, gpointer user_data)
// OK
{
    INIT_UI();
    Greyscale(data);
    //BinarizeColor(data);
}



void callback_brush(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    cursor_state = 1;
    INIT_UI();
    callback_setCursor(data);
}

void callback_rubber(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    cursor_state = 2;
    INIT_UI();
    callback_setCursor(data);
}

void callback_FC(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    //variables definitions
    INIT_UI();
    GError *err = NULL;
    GdkPixbuf *imgPixbuf = gdk_pixbuf_new_from_file("image_test.jpg", &err);
    if(err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
    }

    GET_UI(GtkWidget, FCWindow, "FilterCreator");
    GET_UI(GtkImage, image, "Image_test");

    //test image resize + setting
    int pixbuf_width = gdk_pixbuf_get_width(imgPixbuf) / 2;
    int pixbuf_height = gdk_pixbuf_get_height(imgPixbuf) / 2;


    GdkPixbuf *img2 = gdk_pixbuf_scale_simple(imgPixbuf,
    pixbuf_width, pixbuf_height, GDK_INTERP_BILINEAR);

    gtk_image_clear(image);
    gtk_image_set_from_pixbuf(image, img2);

    //show the filter creator windowjust
    gtk_widget_show(FCWindow);
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
*/

void callback_tinter(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkColorChooser, colorChooser, "ColorTinter");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    if (lay == NULL)
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
    factor = (guchar)(rgba.alpha * 100);

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


void callback_colorfull(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    Colorfull(data);
}


void callback_negative(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    Negative(data);
}

void callback_lightness(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    Lightness(data);
}
/*
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
void reset_cursor(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    resetCursor(data);
    cursor_state = 0; // 0 ==> normal cursor
}

void callback_saveFilter(UNUSED GtkButton *btn, gpointer user_data)
{
    Save_filter(user_data);
}

void callback_applyFilter(UNUSED GtkFileChooser *btn, gpointer user_data)
{
	//a modifier -> recuprer le fichier du dialog
	INIT_UI();
	GET_UI(GtkEntry, entry, "FilterFilename");
	gchar *filename = (gchar*)gtk_entry_get_text (entry);
    Apply_user_filter(filename, user_data);
    GET_UI(GtkWidget, filterchooser, "filterChooser");
    gtk_widget_hide(filterchooser);
}
