#include "callback.h"


int open_confirm_quit_without_saving_dialog(gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkWindow, window, "MainWindow");
    GtkWidget *dialog;
    gint res;

    dialog = gtk_dialog_new_with_buttons(NULL,
                                        window,
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        ("_Annuler"), 0,
                                        ("_Sauvegarder"), 1,
                                        ("_Confirmer"), 2,
                                        NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Quitter sans sauvegarder ?");
    gtk_container_add(GTK_CONTAINER(content), label);
    gtk_widget_set_margin_start(label, 10);
    gtk_widget_set_margin_end(label, 10);
    gtk_widget_set_margin_top(label, 10);
    gtk_widget_set_margin_bottom(label, 10);
    gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_widget_show_all(dialog);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return res;
}

void callback_open(UNUSED GtkMenuItem *menu, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    int confirm = open_confirm_quit_without_saving_dialog(user_data);

    if (confirm == 0)
    {
        return;
    }
    else if (confirm == 1)
    {
        callback_save_project(NULL, user_data);
    }



    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          window,
                                          action,
                                          ("_Annuler"),
                                          GTK_RESPONSE_CANCEL,
                                          ("_Ouvrir"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern(filter, "*.gmpf");
    gtk_file_filter_add_mime_type(filter, "image/*");
    gtk_file_chooser_set_filter(fileChooser, filter);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        char *ext = get_extension(filename);
        D_PRINT("filechoose filename: %s, extension: %s", filename, ext);
        if (!strcmp(ext, "gmpf"))
        {
            D_PRINT("loading project...", NULL);
            char err = load_project (flowbox, filename);
            if (err)
                D_PRINT("Uable to load project", NULL);
            D_PRINT("Project loaded !", NULL);
            if (layermngr->filename != NULL)
            {
                free(layermngr->filename);
            }
            layermngr->filename = filename;
        }
        else
        {
            load_image_cairo(filename, user_data);
            g_free (filename);
        }
    }

    int max_width = layermngr->size.w;
    int max_height = layermngr->size.h;

    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
    gtk_widget_queue_draw(da);

    gtk_widget_destroy (dialog);
}

void callback_rotate_angle(GtkEntry *entry, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return;

    lay->rotate_angle = atoi(gtk_entry_get_text(entry));
    cairo_surface_t *surface = cairo_surface_create_similar_image(lay->surface,
                                                            CAIRO_FORMAT_ARGB32,
                                                            lay->size.w,
                                                            lay->size.h);
    lay->cr = cairo_create(surface);
    cairo_save(lay->cr);
    cairo_translate(lay->cr, lay->size.w / 2, lay->size.h / 2);
    cairo_rotate(lay->cr, RAD_FROM_DEG(lay->rotate_angle));
    cairo_translate(lay->cr, -lay->size.w / 2, -lay->size.h / 2);
    gdk_cairo_set_source_pixbuf(lay->cr, lay->image, 0, 0);
    cairo_paint(lay->cr);
    cairo_restore(lay->cr);
    cairo_destroy(lay->cr); lay->cr = NULL;
    while (cairo_surface_get_reference_count(lay->surface) > 0)
        cairo_surface_destroy(lay->surface);
    lay->surface = surface;

    // REFRESH_IMAGE(lay);
    // lay->rotate_angle = 0;

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

void callback_rotate(UNUSED GtkMenuItem *menuitem, UNUSED gpointer user_data)
{
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
}

void callback_hideWidget(GtkWidget *widget, UNUSED gpointer user_data)
{
    gtk_widget_hide(widget);
}

void callback_hideParent(UNUSED GtkWidget *widget, GtkWidget *parent)
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
    gtk_widget_hide (dialog);
}

void callback_adjust_scale(GtkEntry *entry, gpointer user_data)
{
    const gchar *s = gtk_entry_get_text (entry);
    float scaleValue = atof(s) / 100;
    adjust_scale (scaleValue, scaleValue, user_data);
}

void callback_export(UNUSED GtkWidget *menuitem, gpointer user_data)
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
                                         ("Untitled.png"));

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (chooser);
        int res = set_extension(&filename, "png");
        D_PRINT("f: %s, ext: %s, checked: %i", filename, "png", res);
        export_cairo_to_png(filename, user_data);
        g_free(filename);
    }

    gtk_widget_destroy (dialog);
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
    gtk_file_chooser_set_filename(filename, NULL);

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
        circular_brush(widget, lay->cr, x - lay->pos.x, y - lay->pos.y,lay->pos.x, lay->pos.y, layermngr->brush_size,
            0, 0, 0, 0, lay->scale_factor.x, lay->scale_factor.y,
            layermngr);
        //end brush zone
        cairo_destroy(lay->cr);
    }
}

void color_picker (UNUSED GtkWidget *widget, gdouble x, gdouble y, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkColorChooser, colorChooser, "ColorTinter");
    pick_color_on_screen(x, y, colorChooser, data);
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
    GET_UI(GtkSpinButton, button, "RotateDegreeSpinButton");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    layermngr->pos.x = -1;
    layermngr->pos.y = -1;

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return FALSE;

    REFRESH_IMAGE(lay);
    lay->rotate_angle = 0;
    gtk_spin_button_set_value(button, lay->rotate_angle);

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
    {
        layermngr->surface = lay->surface;
    }
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

    else if (event->button == GDK_BUTTON_PRIMARY & tool == COLOR_PICKER)
        color_picker (widget, event->x, event->y, user_data);

    if (tool == COLOR_KILLER && (event->state & GDK_BUTTON1_MASK))
        kill_color(widget, user_data);
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

void callback_adjust_rotate_value(UNUSED GtkWidget *widget, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkSpinButton, button, "RotateDegreeSpinButton");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return;
    gtk_spin_button_set_value(button, lay->rotate_angle);
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
                // cairo_translate(cr, lay->size.w / 2, lay->size.h / 2);
                // cairo_rotate(cr, RAD_FROM_DEG(lay->rotate_angle));
                // cairo_translate(cr, -lay->size.w / 2, -lay->size.h / 2);
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
        case '6': tool = COLOR_KILLER;
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
    if (!lay->filename)
    {
        lay->image = new_pixbuf_standardized(&lay->size);
        lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 0, NULL);
        REFRESH_IMAGE(lay);
    }
    else
    {
        cairo_surface_t *surface = cairo_surface_create_similar_image(lay->surface,
                                CAIRO_FORMAT_ARGB32, lay->size.w, lay->size.h);
        cairo_t *cr = cairo_create(surface);
        cairo_set_source_surface(cr, lay->surface, 0, 0);
        cairo_paint(cr);
        cairo_surface_destroy(lay->surface);
        lay->surface = surface;
        REFRESH_IMAGE(lay);
    }

    gtk_widget_hide(window);
}

void callback_save_project(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->filename == NULL)
        callback_save_under_project(NULL, user_data);
    else
    {
        D_PRINT("name: %s", layermngr->filename);
        char err = save_project(flowbox, (const char*)layermngr->filename);
        if (err)
        {
            D_PRINT("Unable to save project", NULL);
        }
    }
}

void callback_save_under_project(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    GtkWidget *dialog;
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

    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern(filter, "*.gmpf");
    gtk_file_chooser_set_filter(fileChooser, filter);

    gtk_file_chooser_set_do_overwrite_confirmation (fileChooser, TRUE);

    gtk_file_chooser_set_current_name (fileChooser,
                                         ("Untitled.gmpf"));

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename (fileChooser);
        if (layermngr->filename)
            free(layermngr->filename);
        layermngr->filename = filename;
        char err = save_project(flowbox, (const char*)filename);
        if (err)
        {
            D_PRINT("Unable to save project", NULL);
        }
        D_PRINT("filename after saved: %s", layermngr->filename);
    }

    gtk_widget_destroy (dialog);
}

void callback_load_project(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

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
    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern(filter, "*.gmpf");
    gtk_file_chooser_set_filter(fileChooser, filter);

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

    int max_width = layermngr->size.w;
    int max_height = layermngr->size.h;

    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
    gtk_widget_queue_draw(da);

    gtk_widget_destroy (dialog);
}

void callback_save_layer(UNUSED GtkMenuItem *menuitem, gpointer user_data)
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
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern(filter, "*.lay");
    gtk_file_chooser_set_filter(chooser, filter);

    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_current_name (chooser,
                                         ("Untitled.lay"));

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (chooser);
        char err = saving_layer(flowbox, (const char*)filename);
        if (err)
        {
            D_PRINT("Unable to save project", NULL);
        }
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

void callback_load_layer(UNUSED GtkMenuItem *menuitem, gpointer user_data)
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

    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern(filter, "*.lay");
    gtk_file_chooser_set_filter(chooser, filter);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        char err = loading_layer (flowbox, filename);
        if (err)
            D_PRINT("Uable to load project", NULL);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);

}

void load_image_cairo(char *filename, gpointer user_data)
{
    INIT_UI();
    GError *error = NULL;

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
    layermngr->filename = NULL;
}

void callback_brush(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr->tool = PAINTER;
    callback_setCursor(data);
}

void callback_color_killer(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr->tool = COLOR_KILLER;
    callback_setCursor(data);
}

void callback_rubber(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr->tool = ERAISER;
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

void callback_binarize(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Binarize, data);
}

void callback_binarize_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Binarize, data);
}

void callback_binarize_color(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(BinarizeColor, data);
}

void callback_binarize_color_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(BinarizeColor, data);
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

    double *mat = malloc(sizeof(double) * 9);
    switch (number)
    {
        case 0 :
        mat[0] = mat[1] = mat[2] = mat[3] = mat[5] = mat[6] = mat[7] = mat[8] = -1;
        mat[4] = 8;
        break;
        case 1 :
        mat[0] = mat[2] = mat[6] = mat[8] = 0;
        mat[1] = mat[3] = mat[5] = mat[7] = -1;
        mat[4] = 5;
        break;
        default :
        mat[0] = mat[1] = mat[2] = mat[3] = mat[4] = mat[5] = mat[6] = mat[7] = mat[8] = (double) 1/9;
        break;
    }
    Convolute(data, mat);
}


void callback_grey(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Greyscale, data);
}

void callback_grey_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Greyscale, data);
}

void callback_tinter(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkColorChooser, chooser, "ColorTinter");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    Tinter(lay, chooser);
}


void callback_colorfull(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkColorChooser, chooser, "ColorTinter");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    Colorfull(lay, chooser);
}


void callback_negative(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Negative, data);
}

void callback_negative_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Negative, data);
}

void callback_darkness(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Darkness, data);
}

void callback_darkness_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Darkness, data);
}

void callback_lightness(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Lightness, data);
}

void callback_lightness_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Lightness, data);
}

void callback_equalize(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Equalize, data);
}

void callback_equalize_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Equalize, data);
}

void callback_equalize_color(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Equalize_color, data);
}

void callback_equalize_color_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Equalize_color, data);
}

void callback_horizontale(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Horizontale, data);
}

void callback_horizontale_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Horizontale, data);
}

void callback_verticale(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Verticale, data);
}

void callback_verticale_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Verticale, data);
}

void reset_cursor(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    resetCursor(data);
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr->tool = INCORECT_TOOL;
}

void callback_saveFilter(UNUSED GtkButton *btn, gpointer user_data)
{
    INIT_UI();
    Save_filter(user_data);
    GET_UI(GtkWidget, parent, "FilterCreator");
    gtk_widget_hide(parent);
}

void callback_warm(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
	GMPF_filter_apply_to_selected_layer_color(Color_balance, 100, 255, 255, data);
}

void callback_warm_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
	GMPF_filter_apply_to_all_layer_color(Color_balance, 100, 255, 255, data);
}

void callback_green(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
	GMPF_filter_apply_to_selected_layer_color(Color_balance, 255, 100, 255, data);
}

void callback_green_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer_color(Color_balance, 255, 100, 255, data);
}

void callback_cold(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
	GMPF_filter_apply_to_selected_layer_color(Color_balance, 255, 255, 100, data);
}

void callback_cold_all(UNUSED GtkMenuItem *menuitem, gpointer user_data)
{
    INIT_UI();
	GMPF_filter_apply_to_all_layer_color(Color_balance, 255, 255, 100, data);
}

void callback_applyFilter(UNUSED GtkWidget *btn, gpointer user_data)
{
    //a modifier -> recuprer le fichier du dialog
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Choisir un filtre",
                                          window,
                                          action,
                                          ("Annuler"),
                                          GTK_RESPONSE_CANCEL,
                                          ("Ouvrir"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        Apply_user_filter(filename, user_data);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}
