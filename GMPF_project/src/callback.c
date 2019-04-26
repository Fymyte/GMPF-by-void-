#include "callback.h"

/*
 * Open a dialog with 3 buttons: "Annuler", "Savegarder" and "Confirmer"
 * (Return: 0 for "Annuler", 1 for "Sauverager" and 2 for "Confirmer")
 */
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
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention");
    gtk_window_set_deletable(GTK_WINDOW(dialog), FALSE);
    // gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_widget_show_all(dialog);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return res;
}


/*
 * Load a CSS styleSheet located at filename
 * (Do nothing if filename is incorect)
 */
void load_theme(GdkScreen  *screen,
                const char *filename)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *err = NULL;
    GFile *file = g_file_new_for_path(filename);

    gtk_css_provider_load_from_file(provider, file, &err);
    if (err)
    {
        PRINTERR;
        D_PRINT("%s", err->message)
        return;
    }
    gtk_style_context_add_provider_for_screen(screen,
                                      GTK_STYLE_PROVIDER(provider),
                                      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}


/*
 * Callback to load a GTK-CSS theme
 */
void callback_load_theme(UNUSED GtkWidget *widget,
                         gpointer          user_data)
{
    INIT_UI();
    GET_UI(GtkWindow, window, "MainWindow");
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
    gtk_file_filter_add_pattern(filter, "*.css");
    gtk_file_chooser_set_filter(fileChooser, filter);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        load_theme(gtk_window_get_screen(window), filename);
    }
    gtk_widget_destroy(dialog);
}


/*
 * Open a new GMPF or IMAGE file
 * (Open a Dialog and open the selected file)
 */
void open_new_file(GtkWindow      *window,
                   GMPF_LayerMngr *layermngr,
                   GtkFlowBox     *flowbox)
{
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
        if (!strcmp(ext, "gmpf"))
        {
            if (layermngr->filename != NULL)
                free(layermngr->filename);
            char err = load_project (flowbox, filename);
            if (err)
                D_PRINT("Uable to load project", NULL);
            layermngr->filename = filename;
            GMPF_saved_state_set_is_saved(flowbox, 1);
            int width = layermngr->size.w;
            int height = layermngr->size.h;
            char *title = malloc (sizeof(char) * (strlen(filename) + 30));
            sprintf(title, "GMPF - %s : %d * %d", filename, width, height);
            gtk_window_set_title(window, (const char*)title);
        }
        else
        {
            load_image_cairo(window, layermngr, flowbox, filename);
            g_free (filename);
        }
    }
    gtk_widget_destroy(dialog);
}


/*
 * Callback to open a new file
 * (Check if the project is saved and open a confirm dialog if not)
 */
void callback_open(UNUSED GtkMenuItem *menu,
                   gpointer            user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    int confirm = 2;
    if (!GMPF_saved_state_get_is_saved(flowbox))
        confirm = open_confirm_quit_without_saving_dialog(user_data);

    if (confirm == 0)
    {
        return;
    }
    else if (confirm == 1)
    {
        if (!GMPF_save_project(user_data))
            return;
    }

    open_new_file(window, layermngr, flowbox);

    int max_width = layermngr->size.w;
    int max_height = layermngr->size.h;

    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
    gtk_widget_queue_draw(da);
}


/*
 * Rotate the layer of "rotate_angle" degree
 */
void layer_rotate_angle(GMPF_Layer *lay,
                        int         rotate_angle)
{
    lay->rotate_angle = rotate_angle;
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
}


/*
 * Callback to rotate the selected layer of the given degree
 * (Refresh the displayed surface after rotation)
 */
void callback_rotate_angle(GtkEntry *entry,
                           gpointer  user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return;

    layer_rotate_angle(lay, atoi(gtk_entry_get_text(entry)));

    gtk_widget_queue_draw(da);
}


/*
 * Rotate all the Layer from the LayerMngr's list of Layer from "angle" angle
 */
void layer_rotate_angle_all(int             angle,
                            GMPF_LayerMngr *layermngr)
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


/*
 * Callback to rotate all layer from the given angle
 * (Refresh the displayed surface after rotation)
 */
void callback_rotate_angle_all(GtkEntry *entry,
                               gpointer  user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    const gchar *s = gtk_entry_get_text (entry);
    layer_rotate_angle_all (atoi(s), layermngr_get_layermngr(flowbox));

    gtk_widget_queue_draw(da);
}


/*
 * Callback to adjust the displayed rotate angle according to the selected Layer
 */
void callback_adjust_rotate_value(UNUSED GtkWidget *widget,
                                  gpointer          user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkSpinButton, button, "RotateDegreeSpinButton");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    return;
    gtk_spin_button_set_value(button, lay->rotate_angle);
}


/*
 * Set if the layer is displayed on the surface or not
 */
void layer_set_visible (GMPF_Layer *lay,
                        gboolean    isvisible)
{
    lay->isvisible = isvisible;
}


/*
 * Callback to set the visibility of the selected Layer
 * (Do nothing if there is no selected Layer)
 * (Refresh the displayed surface after rotation)
 */
void callback_layer_set_visible(GtkToggleButton *button,
                                gpointer         user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");

    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    if (!layer)
        D_PRINT("Unable to get selected layer", NULL);

    layer_set_visible(layer, gtk_toggle_button_get_active(button));
    gtk_widget_queue_draw(da);
}


/*
 * Callback to move down the selected Layer in the list of Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_layer_move_down(UNUSED GtkWidget *widget,
                              gpointer          user_data)
{
    SGlobalData *data = (SGlobalData *)user_data;
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    layermngr_move_down_selected_layer(flowbox);
    gtk_widget_queue_draw(da);
}


/*
 * Callback to move up the selected Layer in the list of Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_layer_move_up(UNUSED GtkWidget *widget,
                            gpointer          user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    layermngr_move_up_selected_layer(flowbox);
    gtk_widget_queue_draw(da);;
}


/*
 * Callback to hide the "widget" widget
 */
void callback_hideWidget(GtkWidget      *widget,
                         UNUSED gpointer user_data)
{
    gtk_widget_hide(widget);
}


/*
 * Callback to hide the "parent" widget
 */
void callback_hideParent(UNUSED GtkWidget *widget,
                         GtkWidget        *parent)
{
    gtk_widget_hide(parent);
}


/*
 * Callback to show the "A propos" dialod window
 */
void callback_about (UNUSED GtkMenuItem *menuitem,
                     gpointer            user_data)
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


/*
 * Set the scale value of all Layer from the list of Layer
 * (Return: The scaled size of the surface)
 */
GMPF_Size adjust_scale(double          scale_x,
                       double          scale_y,
                       GMPF_LayerMngr *layermngr)
{
    GMPF_Size size = {.w = 0, .h = 0};

    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            lay->scale_factor.x = scale_x;
            lay->scale_factor.y = scale_y;

            if (lay->size.w * scale_x > size.w)
            size.w = lay->size.w * scale_x;
            if (lay->size.h * scale_y > size.h)
            size.h = lay->size.h * scale_y;

            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    return size;
}


/*
 * Callback to adjust the scale value of all Layer from the list of Layer
 * (Refresh the display after scaling)
 */
void callback_adjust_scale(GtkEntry *entry,
                           gpointer  user_data)
{
    INIT_UI();
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    const gchar *s = gtk_entry_get_text (entry);
    float scaleValue = atof(s) / 100;
    GMPF_Size size = adjust_scale (scaleValue, scaleValue, layermngr);
    int max_width = size.w;
    int max_height = size.h;

    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
    gtk_widget_queue_draw(da);
}


/*
 * Callback to export the project to PNG format
 * (Open a dialog to choose the path for the file to save)
 */
void callback_export(UNUSED GtkWidget *menuitem,
                     gpointer          user_data)
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
        if (set_extension(&filename, "png"))
        {
            D_PRINT("Unable to set extension", NULL);
        }
        export_cairo_to_png(filename, user_data);
        g_free(filename);
    }

    gtk_widget_destroy (dialog);
}


/*
 * Callback to resize the brush size of the LayerMngr
 */
void callback_resize_brush(GtkEntry *entry,
                           gpointer  user_data)
{
    INIT_UI();
    GMPF_LayerMngr *layermngr = NULL;
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    layermngr = layermngr_get_layermngr(flowbox);

    const gchar *s = gtk_entry_get_text (entry);
    float size = atof(s);
    layermngr->brush_size = size;
}


/*
 * Callback to show the "Ajouter un calque" window
 * (Set all the fileds to there default value)
 */
void callback_show_layer_window(UNUSED GtkWidget *widget,
                                gpointer          user_data)
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


/*
 * Clear the LayerMngr surface
 * (Set all pixel's alpha channel to zero)
 */
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


/*
 * Create a new surface of the appropriate size to store our scribbles
 */
gboolean callback_configure_event (UNUSED GtkWidget         *widget,
                                   UNUSED GdkEventConfigure *event,
                                   gpointer                  user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (!layermngr)
        D_PRINT("unable to get layermngr", NULL);

    layermngr->surface = NULL;
    return TRUE;
}


/*
 * Draw a circle on the surface at the given position
 * (The color will be take from the colorChooser of the interface)
 * (The diameter of the circle will be taken from the brush size)
 */
void draw_brush (GtkWidget *widget,
                 gdouble    x,
                 gdouble    y,
                 gpointer   user_data)
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


/*
 * Eraise the circle of pixels on the surface at the given position
 * (The diameter of the circle will be taken from the brush size)
 */
void draw_rubber (GtkWidget *widget,
                  gdouble    x,
                  gdouble    y,
                  gpointer   user_data)
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


/*
 * Pick the color on surface at the given position
 * (The picked color will be set to the colorChooser of the application)
 */
void color_picker (UNUSED GtkWidget *widget,
                   gdouble           x,
                   gdouble           y,
                   gpointer          user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkColorChooser, colorChooser, "ColorTinter");
    pick_color_on_screen(x, y, colorChooser, data);
}


/*
 * Callback to set the cursor to the right icon
 * (when the cursor enter the surface)
 */
gboolean callback_enter_notify_event (UNUSED GtkWidget *widget,
                                      UNUSED GdkEvent  *event,
                                      gpointer          user_data)
{
    INIT_UI();
    callback_setCursor(data);
    return TRUE;
}


/*
 * Callback to set the cursor to the right icon
 * (when the cursor leave the surface)
 */
gboolean callback_leave_notify_event (UNUSED GtkWidget *widget,
                                      UNUSED GdkEvent   *event,
                                      gpointer           user_data)
{
    INIT_UI();
    resetCursor(data);
    return TRUE;
}


/*
 * Callback that handle the button pressed events
 * (Call the right function depending of the selected tool from the LayerMngr)
 */
gboolean callback_button_press_event (GtkWidget      *widget,
                                      GdkEventButton *event,
                                      gpointer        user_data)
{
    INIT_UI();

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);


    GMPF_Tool tool = layermngr->tool;

    if (event->button == GDK_BUTTON_PRIMARY & tool == PAINTER)
        draw_brush (widget, event->x, event->y, user_data);

    else if (event->button == GDK_BUTTON_PRIMARY & tool == ERAISER)
        draw_rubber (widget, event->x, event->y, user_data);

    else if (event->button == GDK_BUTTON_PRIMARY & tool == COLOR_PICKER)
        color_picker (widget, event->x, event->y, user_data);

    else if (event->button == GDK_BUTTON_PRIMARY & tool == COLOR_KILLER)
        kill_color(widget, .2, user_data);

    /* We've handled the event, stop processing */
    return TRUE;
}


/*
 * Callback that handle the button realeased events
 * (Reset the positon of the LayerMngr, update the SavedState of the project and
 * refresh the icon of the selected Layer)
 */
gboolean callback_button_release_event(UNUSED GtkWidget      *widget,
                                       UNUSED GdkEventButton *event,
                                       gpointer               user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkSpinButton, button, "RotateDegreeSpinButton");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    layermngr->pos.x = -1;
    layermngr->pos.y = -1;
    GMPF_Tool tool = layermngr->tool;
    if (tool == PAINTER || tool == ERAISER)
    GMPF_saved_state_set_is_saved(flowbox, 0);
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    return FALSE;

    REFRESH_IMAGE(lay);

    lay->rotate_angle = 0;
    gtk_spin_button_set_value(button, lay->rotate_angle);

    return TRUE;
}


/*
 * Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
gboolean callback_motion_notify_event (GtkWidget      *widget,
                                       GdkEventMotion *event,
                                       gpointer        user_data)
{
    INIT_UI();

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Tool tool = layermngr->tool;

    if (tool == PAINTER && (event->state & GDK_BUTTON1_MASK))
        draw_brush (widget, event->x, event->y, user_data);
    if (tool == ERAISER && (event->state & GDK_BUTTON1_MASK))
        draw_rubber (widget, event->x, event->y, user_data);

    /* We've handled it, stop processing */
    return TRUE;
}


/*
 * Callback to redraw all Layer of the Layer list in the right order on top of
 * eatch other
 */
void callback_on_draw_event(UNUSED GtkWidget *widget,
                            cairo_t *cr,
                            UNUSED gpointer user_data)
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
                cairo_set_source_surface (cr, lay->surface, (double)lay->pos.x, (double)lay->pos.y);
                cairo_paint(cr);
                cairo_restore(cr);
            }
            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    // if (layermngr->surface != NULL)
    // {
    //     D_PRINT("layermngr->surface too", NULL);
    //     cairo_save(cr);
    //     cairo_set_source_surface (cr, layermngr->surface, 0, 0);
    //     cairo_paint(cr);
    //     cairo_restore(cr);
    // }
}


/*
 * Calback to select the right tool according to the pressed button in the
 * ToolBar
 */
void callback_select_tool(GtkWidget *widget,
                          gpointer   user_data)
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
        case '4': tool = COLOR_KILLER;
                         break;
        case '5': tool = SELECTOR;
                         break;
        case '6': tool = SELECTOR_FREE;
                         break;
        default : tool = INCORECT_TOOL;
        D_PRINT("Unknown tool", NULL);
    }
    layermngr->tool = tool;
}


/*
 * Callback to add a new layer in the list from the "Ajouter un calque" window
 * (Refresh the displayed image of the layer and hide the window)
 */
void callback_add_custom_layer(UNUSED GtkWidget *widget,
                               gpointer          user_data)
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


/*
 * Save the project at the saved filename of the LayerMngr and call the
 * "GMPF_save_under_project()" if it is NULL
 */
gboolean GMPF_save_project(gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->filename == NULL)
        return GMPF_save_under_project(user_data);
    else
    {
        char err = save_project(flowbox, (const char*)layermngr->filename);
        if (err)
        {
            D_PRINT("Unable to save project", NULL);
            GMPF_saved_state_set_is_saved(flowbox, 0);
            return FALSE;
        }
    }
    GMPF_saved_state_set_is_saved(flowbox, 1);
    return TRUE;
}


/*
 * Callback to save the project
 */
void callback_save_project(UNUSED GtkMenuItem *menuitem,
                           gpointer            user_data)
{
    GMPF_save_project(user_data);
}


/*
 * Save the project at the selected filename
 * (Open a dialog to choose the path where to save the project)
 */
gboolean GMPF_save_under_project(gpointer user_data)
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
        gtk_widget_destroy (dialog);
        char err = save_project(flowbox, (const char*)filename);
        if (err)
        {
            D_PRINT("Unable to save project", NULL);
            GMPF_saved_state_set_is_saved(flowbox, 0);
            return FALSE;
        }
        GMPF_saved_state_set_is_saved(flowbox, 1);
        return TRUE;
    }
    gtk_widget_destroy (dialog);
    return FALSE;
}


/*
 * Callback to save the project at the selected filename
 */
void callback_save_under_project(UNUSED GtkMenuItem *menuitem,
                                 gpointer            user_data)
{
    GMPF_save_under_project(user_data);
}


/*
 * Callback to save the selected layer in a specific file type
 * (Do nothing if there is no selected Layer)
 */
void callback_save_layer(UNUSED GtkMenuItem *menuitem,
                         gpointer            user_data)
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


/*
 * Callback to load the selected layer in the dialog
 * (Do nothing if there is no selected Layer)
 */
void callback_load_layer(UNUSED GtkMenuItem *menuitem,
                         gpointer            user_data)
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


/*
 * Load an image from filename and ad it to the list of Layer
 * (Do nothing if the filename is incorect)
 */
void load_image_cairo(GtkWindow *window,
                      GMPF_LayerMngr *layermngr,
                      GtkFlowBox *flowbox,
                      char *filename)
{
    GError *error = NULL;
    layermngr_add_new_layer(flowbox, filename);

    int max_width  = layermngr->size.w;
    int max_height = layermngr->size.h;
    //
    int width, height;
    layermngr->image  = gdk_pixbuf_new_from_file(filename, &error);
    width  = gdk_pixbuf_get_width  (layermngr->image);
    height = gdk_pixbuf_get_height (layermngr->image);
    char *title = malloc (sizeof(char) * (strlen(filename) + 30));
    sprintf(title, "GMPF - %s : %d * %d", filename, width, height);
    gtk_window_set_title(window, (const char*)title);
    GMPF_saved_state_set_is_saved(flowbox, 0);

    if (width > max_width)
        max_width = width;
    if (height > max_height)
        max_height = height;

    layermngr->size.w = max_width;
    layermngr->size.h = max_height;

    layermngr->surface = gdk_cairo_surface_create_from_pixbuf(layermngr->image, 0, NULL);

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


/*
 * Show the "Créateur de filtre" window
 * (init all its variables to there default value)
 */
void callback_FC(UNUSED GtkMenuItem *menuitem,
                 gpointer            user_data)
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
 * Calback to apply "Binarisation" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_binarize(UNUSED GtkMenuItem *menuitem,
                       gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Binarize, data);
}


/*
 * Calback to apply "Binarisation" filter to all Layer
 */
void callback_binarize_all(UNUSED GtkMenuItem *menuitem,
                           gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Binarize, data);
}


/*
 * Calback to apply "Binarisation Coloré" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_binarize_color(UNUSED GtkMenuItem *menuitem,
                             gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(BinarizeColor, data);
}


/*
 * Calback to apply "Binarisation Coloré" filter to the Layer
 */
void callback_binarize_color_all(UNUSED GtkMenuItem *menuitem,
                                 gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(BinarizeColor, data);
}


/*
 * Calback to apply the correct convolution matrix according to the pressed
 * button to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_convolute_f(UNUSED GtkMenuItem *menuitem,
                          gpointer            user_data)
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


/*
 * Calback to apply "Niveau de gris" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_grey(UNUSED GtkMenuItem *menuitem,
                   gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Greyscale, data);
}


/*
 * Calback to apply "Niveau de gris" filter to all Layer
 */
void callback_grey_all(UNUSED GtkMenuItem *menuitem,
                       gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Greyscale, data);
}


/*
 * Calback to apply "Teinture" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_tinter(UNUSED GtkMenuItem *menuitem,
                     gpointer            user_data)
{
    INIT_UI();
    GET_UI(GtkColorChooser, chooser, "ColorTinter");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    Tinter(lay, chooser);
}


/*
 * Calback to apply "Accentuation des couleurs" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_colorfull(UNUSED GtkMenuItem *menuitem,
                        gpointer            user_data)
{
    INIT_UI();
    GET_UI(GtkColorChooser, chooser, "ColorTinter");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    Colorfull(lay, chooser);
}


/*
 * Calback to apply "Négatif" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_negative(UNUSED GtkMenuItem *menuitem,
                       gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Negative, data);
}


/*
 * Calback to apply "Négatif" filter to the all Layer
 */
void callback_negative_all(UNUSED GtkMenuItem *menuitem,
                           gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Negative, data);
}


/*
 * Calback to apply "Luminosité-" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_darkness(UNUSED GtkMenuItem *menuitem,
                      gpointer             user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Darkness, data);
}


/*
 * Calback to apply "Luminosité-" filter to all Layer
 */
void callback_darkness_all(UNUSED GtkMenuItem *menuitem,
                           gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Darkness, data);
}


/*
 * Calback to apply "Luminosité+" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_lightness(UNUSED GtkMenuItem *menuitem,
                        gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Lightness, data);
}


/*
 * Calback to apply "Luminosité+" filter to all Layer
 */
void callback_lightness_all(UNUSED GtkMenuItem *menuitem,
                            gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Lightness, data);
}


/*
 * Calback to apply "Egalisation" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_equalize(UNUSED GtkMenuItem *menuitem,
                       gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Equalize, data);
}


/*
 * Calback to apply "Egalisation" filter to all Layer
 */
void callback_equalize_all(UNUSED GtkMenuItem *menuitem,
                           gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Equalize, data);
}


/*
 * Calback to apply "Egalisation Coloré" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_equalize_color(UNUSED GtkMenuItem *menuitem,
                             gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Equalize_color, data);
}


/*
 * Calback to apply "Egalisation Coloré" filter to all Layer
 */
void callback_equalize_color_all(UNUSED GtkMenuItem *menuitem,
                                 gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Equalize_color, data);
}


/*
 * Calback to apply "Mirroir Horizontal" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_horizontale(UNUSED GtkMenuItem *menuitem,
                          gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Horizontale, data);
}


/*
 * Calback to apply "Mirroir Horizontal" filter to all Layer
 */
void callback_horizontale_all(UNUSED GtkMenuItem *menuitem,
                              gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Horizontale, data);
}


/*
 * Calback to apply "Mirroir Vertical" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_verticale(UNUSED GtkMenuItem *menuitem,
                        gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer(Verticale, data);
}


/*
 * Calback to apply "Mirroir Vertical" filter to all Layer
 */
void callback_verticale_all(UNUSED GtkMenuItem *menuitem,
                            gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer(Verticale, data);
}


void reset_cursor(UNUSED GtkMenuItem *menuitem,
                  gpointer            user_data)
{
    INIT_UI();
    resetCursor(data);
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr->tool = INCORECT_TOOL;
}


/*
 * Callback to save the newly created filter using "Créatteur de Filtre" window
 * (Do noting if no filter has been selected)
 */
void callback_saveFilter(UNUSED GtkButton *btn,
                         gpointer          user_data)
{
    INIT_UI();
    Save_filter(user_data);
    GET_UI(GtkWidget, parent, "FilterCreator");
    gtk_widget_hide(parent);
}


/*
 * Calback to apply "Chauf" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_warm(UNUSED GtkMenuItem *menuitem,
                   gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer_color(Color_balance, 100, 255, 255, data);
}


/*
 * Calback to apply "chaud" filter to all Layer
 */
void callback_warm_all(UNUSED GtkMenuItem *menuitem,
                       gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer_color(Color_balance, 100, 255, 255, data);
}


/*
 * Calback to apply "Vert" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_green(UNUSED GtkMenuItem *menuitem,
                    gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer_color(Color_balance, 255, 100, 255, data);
}


/*
 * Calback to apply "Vert" filter to all Layer
 */
void callback_green_all(UNUSED GtkMenuItem *menuitem,
                        gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_all_layer_color(Color_balance, 255, 100, 255, data);
}


/*
 * Calback to apply "Froid" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_cold(UNUSED GtkMenuItem *menuitem,
                   gpointer            user_data)
{
    INIT_UI();
    GMPF_filter_apply_to_selected_layer_color(Color_balance, 255, 255, 100, data);
}


/*
 * Calback to apply "Froid" filter to all Layer
 */
void callback_cold_all(UNUSED GtkMenuItem *menuitem,
                       gpointer            user_data)
{
    INIT_UI();
	GMPF_filter_apply_to_all_layer_color(Color_balance, 255, 255, 100, data);
}


/*
 * Callback to apply the selected *.ftr file filter to the selcted Layer
 * (Do nothing if there is no selected Layer)
 * (Open a dialog to select the *.ftr file)
 */
void callback_applyFilter(UNUSED GtkWidget *btn,
                          gpointer          user_data)
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


/*
 * Quit the application, free all element and close all window
 */
void GMPFquit(gpointer user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    layermngr_delete(flowbox);
    GMPF_saved_state_destroy(flowbox);
    resetCursor(data);
    gtk_main_quit();
}


/*
 * Callback to quit the application
 * (Ask confirmation before quit)
 */
void callback_quit(UNUSED GtkWidget *widget,
                   gpointer          user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    int confirm = 2;
    if (!GMPF_saved_state_get_is_saved(flowbox))
        confirm = open_confirm_quit_without_saving_dialog(user_data);
    if (confirm == 0)
    {
        return;
    }
    else if (confirm == 1)
        if (!GMPF_save_project(user_data))
            return;
     GMPFquit(user_data);
}


/*
 * Callback ask the user if he want to quit the application with the cross
 * icon on the top right corner of the window
 * (Ask confirmation before quit)
 */
gboolean do_destroy_event(UNUSED GtkWidget *widget,
                          UNUSED GdkEvent * event,
                          gpointer          user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    int confirm = 2;
    if (!GMPF_saved_state_get_is_saved(flowbox))
        confirm = open_confirm_quit_without_saving_dialog(user_data);
    if (confirm == 0)
    {
        return TRUE;
    }
    else if (confirm == 1)
    {
        if (!GMPF_save_project(user_data))
            return TRUE;
    }
    GMPFquit(user_data);
    return FALSE;
}


/*
 * Callback to edit selected layer's properties
 */
void callback_edit_layer_properties(UNUSED GtkWidget *widget,
                                    gpointer          user_data)
{
    INIT_UI();
    GET_UI(GtkEntry, name, "LayerNameEntry1");
    GET_UI(GtkEntry, width, "LayerWidthSpinButton1");
    GET_UI(GtkEntry, height, "LayerHeightSpinButton1");
    GET_UI(GtkEntry, offsetX, "LayerOffsetXSpinButton1");
    GET_UI(GtkEntry, offsetY, "LayerOffsetYSpinButton1");
    GET_UI(GtkFileChooser, filename, "LayerImageFilename1");
    GET_UI(GtkWidget, window, "EditLayerWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    const gchar *n = gtk_entry_get_text(name);
    const gchar *w = gtk_entry_get_text(width);
    const gchar *h = gtk_entry_get_text(height);
    const gchar *x = gtk_entry_get_text(offsetX);
    const gchar *y = gtk_entry_get_text(offsetY);

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    lay->name = (char*)n;
    lay->size.w = atoi(w);
    lay->size.h = atoi(h);
    lay->pos.x = atoi(x);
    lay->pos.y = atoi(y);

    cairo_surface_t *surface = cairo_surface_create_similar_image(lay->surface,
                            CAIRO_FORMAT_ARGB32, lay->size.w, lay->size.h);
    cairo_t *cr = cairo_create(surface);
    cairo_set_source_surface(cr, lay->surface, 0, 0);
    cairo_paint(cr);
    cairo_surface_destroy(lay->surface);
    lay->surface = surface;
    REFRESH_IMAGE(lay);


    gtk_widget_hide(window);
}


/*
 * Callback to open the edit layer window and set the properties to their
 * default value
 * (Do nothing if there is no selected Layer)
 */
void callback_open_edit_layer_properties_window(UNUSED GtkWidget *widget,
                                                gpointer   user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return;
    GET_UI(GtkWidget, layer_window, "EditLayerWindow");
    GET_UI(GtkEntry, name, "LayerNameEntry1");
    GET_UI(GtkSpinButton, width, "LayerWidthSpinButton1");
    GET_UI(GtkSpinButton, height, "LayerHeightSpinButton1");
    GET_UI(GtkSpinButton, offsetX, "LayerOffsetXSpinButton1");
    GET_UI(GtkSpinButton, offsetY, "LayerOffsetYSpinButton1");
    GET_UI(GtkFileChooser, filename, "LayerImageFilename1");

    gtk_entry_set_text(name, lay->name != NULL ? lay->name : "");
    gtk_spin_button_set_value(width, lay->size.w);
    gtk_spin_button_set_value(height, lay->size.h);
    gtk_spin_button_set_value(offsetX, lay->pos.x);
    gtk_spin_button_set_value(offsetY, lay->pos.y);
    gtk_file_chooser_set_filename(filename, lay->filename);

    gtk_widget_show(layer_window);
}

/*
 * Callback to clear the LayerMngr and reset it
 */
void callback_clear_GMPF_LayerMngr(UNUSED GtkMenuItem *menuitem,
                                   gpointer            user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    layermngr_clear(flowbox);
}


/*
 * Callback to remove the selected Layer from the Layer list
 * (Do nothing if there is no selected Layer)
 */
void callback_remove_selected_layer(UNUSED GtkMenuItem *menuitem,
                                    gpointer            user_data)
{
    INIT_UI();
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr_delete_selected_layer(flowbox);
    if (!layermngr->layer_list.next)
    {
        layermngr->size.w = 0;
        layermngr->size.h = 0;
        gtk_widget_set_size_request(layout, 0, 0);
        gtk_widget_set_size_request(da, 0, 0);
        gtk_layout_set_size((GtkLayout *)layout, 0, 0);
    }
    gtk_widget_queue_draw(da);
}
