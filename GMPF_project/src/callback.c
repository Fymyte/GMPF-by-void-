#include "callback.h"

extern SGlobalData G_user_data;

char is_file_exist(const char *filename)
{
    FILE *file= fopen(filename, "r");

    if (file)
    { fclose(file); return 1; }

    return 0;
}

/*
 * PURPOSE : Open a dialog with 3 buttons: "Annuler", "Savegarder" and "Confirmer"
 *  PARAMS : UNUSED UNUSED gpointer user_data -
 * RETURNS : int - 0 for "Annuler"
 *                 1 for "Sauvegarder"
 *                 2 for "Confirmer"
 *   NOTES :
 */
char open_confirm_quit_without_saving_dialog(const char *msg)
{
    // gtk_file_filter_add_pattern(filter, "*.gmpf~");
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
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
    if (!msg)
        msg = "Quitter sans sauvegarder ?\nToute modification non sauvegardé sera perdu!";
    GtkWidget *label = gtk_label_new(msg);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(content), label);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context,"window-background");
    context = gtk_widget_get_style_context(content);
    gtk_style_context_add_class(context,"window-background");
    gtk_style_context_add_class(context,"margin-10");
    context = gtk_widget_get_style_context(label);
    gtk_style_context_add_class(context,"window-background-soft");
    gtk_style_context_add_class(context,"margin-20");
    gtk_style_context_add_class(context,"padding-20");
    gtk_style_context_add_class(context,"border-radius-10");
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention");
    gtk_window_set_deletable(GTK_WINDOW(dialog), FALSE);
    gtk_widget_show_all(dialog);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    if (res == 2)
    {
        char *project_filename = GMPF_project_info_get_filename(flowbox);
        if (!project_filename)
        { return res; }

        char *filename = malloc(sizeof(char) * (strlen(project_filename) + 6));
        sprintf(filename, "%s~", project_filename);
        if (remove (filename))
        { D_PRINT("Unable to remove file", NULL); }
        free(filename);
    }
    return res;
}


/*
 * PURPOSE : Open a dialog with 3 buttons: "Annuler", "Savegarder" and "Confirmer"
 *  PARAMS : UNUSED UNUSED gpointer user_data -
 * RETURNS : int - 0 for "Non"
 *                 1 for "Oui"
 *   NOTES :
 */
char open_open_auto_saved_file_dialog()
{
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GtkWidget *dialog;
    gint res;

    dialog = gtk_dialog_new_with_buttons(NULL,
                                        window,
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        ("_Non"), 0,
                                        ("Oui"), 1,
                                        NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Nous avons détecter une version plus récente de ce fichier.\nVoulez-vous la charger ?");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(content), label);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context,"window-background");
    context = gtk_widget_get_style_context(content);
    gtk_style_context_add_class(context,"window-background");
    gtk_style_context_add_class(context,"margin-10");
    context = gtk_widget_get_style_context(label);
    gtk_style_context_add_class(context,"window-background-soft");
    gtk_style_context_add_class(context,"margin-20");
    gtk_style_context_add_class(context,"padding-20");
    gtk_style_context_add_class(context,"border-radius-10");
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention");
    gtk_window_set_deletable(GTK_WINDOW(dialog), FALSE);
    // gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_widget_show_all(dialog);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    return res;
}


/*
 * PURPOSE : Load a CSS styleSheet located at filename
 *  PARAMS : GdkScreen  *screen - The Screen to apply the theme
 *           const char *filename - The location of the theme's file
 * RETURNS : None
 *   NOTES : Do nothing if filename is incorect
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
        PRINTERR ("Unable to load the CSS provider");
        return;
    }
    gtk_style_context_add_provider_for_screen(screen,
                                      GTK_STYLE_PROVIDER(provider),
                                      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}


/*
 * PURPOSE : Open a dialog to choose the CSS theme to load
 *  PARAMS : UNUSED GtkWidget *widget - UNUSED
 *           gpointer          user_data - contain the builder
 * RETURNS : None
 *   NOTES :
 */
void callback_load_theme(UNUSED GtkWidget *widget,
                         UNUSED gpointer   user_data)
{
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
 * PURPOSE : Open a Dialog and open the selected file
 *  PARAMS : GtkWindow      *window - the window to transient the dialog
 *           GMPF_LayerMngr *layermngr - the LayerMngr with the last filename
 *           GtkFlowBox     *flowbox - the flowbox used to laod an image
 * RETURNS : None
 *   NOTES : Use to open a new GMPF or IMAGE file
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
        char *filename = gtk_file_chooser_get_filename(fileChooser);
        gtk_widget_destroy(dialog);
        char *ext = get_extension(filename);
        if (!strcmp(ext, "gmpf"))
        {
            char *tmpfile = malloc(sizeof(char) * (strlen(filename) + 2));
            sprintf(tmpfile, "%s~", filename);
            char open = 0;
            if (is_file_exist(tmpfile))
                open = open_open_auto_saved_file_dialog();
            if (open)
            {
                char err = load_project (flowbox, tmpfile);
                if (err)
                    D_PRINT("Uable to load project", NULL);
                err = save_project(flowbox, filename);
                if (err)
                    D_PRINT("Unable to save project", NULL);

                GMPF_project_info_set_filename(flowbox, filename);

                if (remove(tmpfile))
                { D_PRINT("Unable to remove file", NULL); }
            }
            else
            {
                char err = load_project (flowbox, filename);
                if (err)
                D_PRINT("Uable to load project", NULL);
            }
            free(tmpfile);

            GMPF_saved_state_set_is_saved(flowbox, 1);
            GMPF_Size size = *GMPF_project_info_get_size(flowbox);

            char *title = malloc (sizeof(char) * (strlen(filename) + 30));
            sprintf(title, "GMPF - %s : %ld * %ld", filename, size.w, size.h);
            gtk_window_set_title(window, (const char*)title);
            g_free(title); // Need to free the title of the window ?
            g_free(filename);
        }
        else
        {
            load_image_cairo(window, layermngr, flowbox, filename);
            g_free (filename);
        }
    }
    else
    {
        gtk_widget_destroy(dialog);
    }
}


/*
 * Callback to open a new file
 * (Check if the project is saved and open a confirm dialog if not)
 */
void callback_open(UNUSED GtkMenuItem *menu,
                   UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    int confirm = 2;
    if (!GMPF_saved_state_get_is_saved(flowbox))
        confirm = open_confirm_quit_without_saving_dialog(NULL);

    if (confirm == 0)
    { return; }

    else if (confirm == 1)
    { if (!GMPF_save_project()) return; }

    open_new_file(window, layermngr, flowbox);

    gtk_widget_queue_draw(da);
}


/*
 * Show the New Project window
 */
void callback_new(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkWidget, window, "NewProjectWindow");
    gtk_widget_show(window);
}


/*
 * Test if the string s1 contain s2
 */
bool str_contain (const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if ((s1[i] > 'Z'?s1[i]-'a'+'A':s1[i]) != (s2[i] > 'Z'?s2[i]-'a'+'A':s2[i]))
            return FALSE;
        i++;
    }

    return s2[i] == '\0';
}


void callback_reset_search(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_LAYERMNGR(flowbox);

    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            // D_PRINT("%s %s contain %s", lay->name, str_contain(lay->name, n) ? "" : "not", n);
            gtk_widget_show((GtkWidget*) lay->UIElement);

            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
}


void callback_search(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkEntry, entry, "LayerSearchEntry");
    GET_LAYERMNGR(flowbox);

    const char *n = gtk_entry_get_text(entry);
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        while (lay != NULL)
        {
            // D_PRINT("%s %s contain %s", lay->name, str_contain(lay->name, n) ? "" : "not", n);
            !str_contain(lay->name, n) ?
                gtk_widget_hide((GtkWidget *)lay->UIElement) :
                gtk_widget_show((GtkWidget *)lay->UIElement);


            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
}


/*
 * Create a new project with the size given by the user in the interface
 * (Clear the previous project if they were one)
 */
void callback_new_project(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkEntry, name, "NewProjectNameEntry");
    GET_UI(GtkEntry, width, "NewProjectWidthSpinButton");
    GET_UI(GtkEntry, height, "NewProjectHeightSpinButton");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");

    const gchar *n = gtk_entry_get_text(name);
    const gchar *w = gtk_entry_get_text(width);
    const gchar *h = gtk_entry_get_text(height);

    layermngr_clear(flowbox);

    GMPF_Size size = { .w = atoll(w), .h = atoll(h) };
    GMPF_project_info_set_size(flowbox, size);
    GMPF_Scale scale = *GMPF_project_info_get_scale(flowbox);

    GMPF_Layer *lay = layermngr_add_new_layer(flowbox, NULL);
    layer_set_name(lay, (char*)n);
    lay->size.w = size.w;
    lay->size.h = size.h;

    if (lay->image)
        g_object_unref(lay->image);

    while (cairo_surface_get_reference_count(lay->surface) > 0)
        cairo_surface_destroy(lay->surface);
    lay->image = new_pixbuf_standardized(&lay->size);
    lay->surface = gdk_cairo_surface_create_from_pixbuf(lay->image, 0, NULL);
    REFRESH_IMAGE(lay);

    int max_width = size.w * scale.x;
    int max_height = size.h * scale.y;
    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);

    GMPF_save_under_project();
}


/*
 * Open the resize project window
 */
void callback_open_resize_project_window(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkWidget, window, "ResizeProjectWindow");
    gtk_widget_show(window);
}


/*
 * Resize the project with the dimension given by the user in the interface
 */
void callback_resize_project(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkEntry, width, "ProjectWidthSpinButton");
    GET_UI(GtkEntry, height, "ProjectHeightSpinButton");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");

    const gchar *w = gtk_entry_get_text(width);
    const gchar *h = gtk_entry_get_text(height);

    GMPF_Size size = { .w = atoll(w), .h = atoll(h) };
    GMPF_project_info_set_size(flowbox, size);
    GMPF_Scale scale = *GMPF_project_info_get_scale(flowbox);

    int max_width = size.w * scale.x;
    int max_height = size.h * scale.y;
    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
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
                           UNUSED gpointer user_data)
{
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
                               UNUSED gpointer user_data)
{
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
                                  UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkSpinButton, button, "RotateDegreeSpinButton");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    return;
    gtk_spin_button_set_value(button, lay->rotate_angle);
}


/*
 * Callback to set the visibility of the selected Layer
 * (Do nothing if there is no selected Layer)
 * (Refresh the displayed surface after rotation)
 */
void callback_layer_set_visible(GtkWidget *button,
                                UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");

    GtkWidget *parent = gtk_widget_get_parent(gtk_widget_get_parent(button));
    if (!parent)
    { PRINTERR("Unable to get parent"); return; }
    GMPF_Layer *layer = (GMPF_Layer *) g_object_get_data(G_OBJECT(parent), LAYER_KEY_NAME);
    if (!layer)
    { PRINTERR("Unable to get layer"); return; }

    layer->isvisible = gtk_toggle_button_get_active((GtkToggleButton *)button);
    gtk_widget_queue_draw(da);
}


/*
 * Callback to move down the selected Layer in the list of Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_layer_move_down(UNUSED GtkWidget *widget,
                              UNUSED gpointer   user_data)
{
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
                            UNUSED gpointer   user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    layermngr_move_up_selected_layer(flowbox);
    gtk_widget_queue_draw(da);
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
                     UNUSED gpointer     user_data)
{
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
                           UNUSED gpointer  user_data)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    const gchar *s = gtk_entry_get_text (entry);
    float scaleValue = atof(s) / 100;
    GMPF_Size size = adjust_scale (scaleValue, scaleValue, layermngr);
    GMPF_Scale scale = { .x = scaleValue, .y = scaleValue };
    GMPF_project_info_set_scale(flowbox, scale);
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
                     UNUSED gpointer   user_data)
{
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
        export_cairo_to_png(filename, flowbox);
        g_free(filename);
    }

    gtk_widget_destroy (dialog);
}


/*
 * Callback to resize the brush size of the LayerMngr
 */
void callback_resize_brush(GtkEntry *entry,
                           UNUSED gpointer  user_data)
{
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
                                UNUSED gpointer   user_data)
{
    //variables definitions
    GET_UI(GtkWidget, layer_window, "LayerWindow");
    GET_UI(GtkEntry, name, "LayerNameEntry");
    GET_UI(GtkSpinButton, width, "LayerWidthSpinButton");
    GET_UI(GtkSpinButton, height, "LayerHeightSpinButton");
    GET_UI(GtkSpinButton, offsetX, "LayerOffsetXSpinButton");
    GET_UI(GtkSpinButton, offsetY, "LayerOffsetYSpinButton");
    GET_UI(GtkButton, button, "LayerFilenameButton")
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    g_object_set_data(G_OBJECT(flowbox), FILENAME_KEY_NAME, NULL);
    gtk_button_set_label(button, "(Aucun)");

    GET_LAYERMNGR(flowbox);
    if (!layermngr->layer_list.next)
    {
        GET_UI(GtkWidget, projectwindow, "NewProjectWindow");
        gtk_widget_show(projectwindow);
        return;
    }

    gtk_entry_set_text(name, "");
    GMPF_Size size = *GMPF_project_info_get_size(flowbox);
    gtk_spin_button_set_value(width, size.w);
    gtk_spin_button_set_value(height, size.h);
    gtk_spin_button_set_value(offsetX, 0);
    gtk_spin_button_set_value(offsetY, 0);

    gtk_widget_show(layer_window);
}


void callback_choose_filename_for_layer(UNUSED GtkWidget *widget,
                                        UNUSED gpointer user_data)
{
    GET_UI(GtkWindow, window, "MainWindow");
    GET_UI(GObject, flowbox, "GMPF_flowbox");
    GET_UI(GtkButton, button, "LayerFilenameButton");
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Choisir un fichier image",
                                          window,
                                          action,
                                          ("_Annuler"),
                                          GTK_RESPONSE_CANCEL,
                                          ("_Ouvrir"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_mime_type(filter, "image/*");
    gtk_file_chooser_set_filter(fileChooser, filter);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res)
    {
        char *filename = gtk_file_chooser_get_filename(fileChooser);
        char *nfilename = malloc (sizeof(char) * (strlen(filename) + 1));
        nfilename = strcpy(nfilename, filename);
        free(filename);
        gtk_button_set_label(button, filename_get_name_of_file(nfilename));
        g_object_set_data(flowbox, FILENAME_KEY_NAME, nfilename);
    }
    gtk_widget_destroy(dialog);
}


/*
 * Clear the LayerMngr surface
 * (Set all pixel's alpha channel to zero)
 */
void clear_surface ()
{
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
 * Draw a circle on the surface at the given position
 * (The color will be take from the colorChooser of the interface)
 * (The diameter of the circle will be taken from the brush size)
 */
void draw_brush (GtkWidget *widget,
                 gdouble    x,
                 gdouble    y)
{
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
        if (layermngr->brush == 0)
            circular_brush(widget, lay->cr, x - lay->pos.x, y - lay->pos.y,lay->pos.x, lay->pos.y, layermngr->brush_size,
                (float)color.red, (float)color.green, (float)color.blue,
                (float)color.alpha, lay->scale_factor.x, lay->scale_factor.y,
                layermngr);
        if (layermngr->brush == 1)
            square_brush(widget, lay->cr, x - lay->pos.x, y - lay->pos.y,
                layermngr->brush_size, (float)color.red, (float)color.green,
                (float)color.blue, (float)color.alpha);

        if (layermngr->brush == 2)
            rectangular_brush(widget, lay->cr, x - lay->pos.x, y - lay->pos.y,
                layermngr->brush_size, (float)color.red, (float)color.green,
                (float)color.blue, (float)color.alpha);

        if (layermngr->brush == 3)
            triangle_brush(widget, lay->cr, x - lay->pos.x, y - lay->pos.y,
                layermngr->brush_size, (float)color.red, (float)color.green,
                (float)color.blue, (float)color.alpha);
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
                  gdouble    y)
{
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
void color_picker (gdouble x,
                   gdouble y)
{
    pick_color_on_screen(x, y);
}


/*
 * Callback to set the cursor to the right icon
 * (when the cursor enter the surface)
 */
gboolean callback_enter_notify_event (UNUSED GtkWidget *widget,
                                      UNUSED GdkEvent  *event,
                                      UNUSED gpointer   user_data)
{
    callback_setCursor();
    return TRUE;
}


/*
 * Callback to set the cursor to the right icon
 * (when the cursor leave the surface)
 */
gboolean callback_leave_notify_event (UNUSED GtkWidget *widget,
                                      UNUSED GdkEvent   *event,
                                      UNUSED gpointer    user_data)
{
    GET_UI(GtkWindow, window, "MainWindow");
    resetCursor(window);
    return TRUE;
}


/*
 * Callback that handle the button pressed events
 * (Call the right function depending of the selected tool from the LayerMngr)
 */
gboolean callback_button_press_event (GtkWidget      *widget,
                                      GdkEventButton *event,
                                      UNUSED gpointer user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    GMPF_Tool tool = layermngr->tool;
    if (tool != GMPF_TOOL_SELECTOR)
    { layermngr->pos.x = -1; layermngr->pos.y = -1; }

    if (event->button == GDK_BUTTON_PRIMARY & tool == GMPF_TOOL_PAINTER)
    {
        if (lay) { GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay); }
        draw_brush (widget, event->x, event->y);
    }

    else if (event->button == GDK_BUTTON_PRIMARY & tool == GMPF_TOOL_ERAISER)
    {
        if (lay) { GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay); }
        draw_rubber (widget, event->x, event->y);
    }

    else if (event->button == GDK_BUTTON_PRIMARY & tool == GMPF_TOOL_COLOR_PICKER)
        color_picker (event->x, event->y);

    else if (event->button == GDK_BUTTON_PRIMARY & tool == GMPF_TOOL_COLOR_KILLER)
    {
        if (lay) { GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay); }
        kill_color(lay, .2);
    }

    else if(event->button == GDK_BUTTON_PRIMARY & tool == GMPF_TOOL_SELECTOR)
        { layermngr->pos.x = event->x; layermngr->pos.y = event->y; }

    /* We've handled the event, stop processing */
    return TRUE;
}


/*
 * Callback that handle the button realeased events
 * (Reset the positon of the LayerMngr, update the SavedState of the project and
 * refresh the icon of the selected Layer)
 */
gboolean callback_button_release_event(UNUSED GtkWidget *widget,
                                       GdkEventButton   *event,
                                       UNUSED gpointer   user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkSpinButton, button, "RotateDegreeSpinButton");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    GMPF_Tool tool = layermngr->tool;
    if (!lay)
        return FALSE;

    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        GMPF_Pos pos = { .x = layermngr->pos.x, .y = layermngr->pos.y };
        GMPF_Pos npos = { .x = event->x, .y = event->y };

        if (selector(flowbox, pos, npos))
        { return FALSE; }

        gtk_widget_queue_draw(da);;

        return TRUE;
    }

    REFRESH_IMAGE(lay);

    if (tool == GMPF_TOOL_PAINTER || tool == GMPF_TOOL_ERAISER || lay->rotate_angle)
    {
        GMPF_saved_state_set_is_saved(flowbox, 0);
        lay->rotate_angle = 0;
        gtk_spin_button_set_value(button, lay->rotate_angle);
    }

    return TRUE;
}


/*
 * Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
gboolean callback_motion_notify_event (GtkWidget      *widget,
                                       GdkEventMotion *event,
                                       UNUSED gpointer ser_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Tool tool = layermngr->tool;

    if (tool == GMPF_TOOL_PAINTER && (event->state & GDK_BUTTON1_MASK))
        draw_brush (widget, event->x, event->y);
    else if (tool == GMPF_TOOL_ERAISER && (event->state & GDK_BUTTON1_MASK))
        draw_rubber (widget, event->x, event->y);
    else if (tool == GMPF_TOOL_SELECTOR && (event->state & GDK_BUTTON1_MASK))
    {
        GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
        if (!lay)
            return FALSE;

        GMPF_Pos pos = { .x = layermngr->pos.x, .y = layermngr->pos.y};
        GMPF_Pos npos = { .x = event->x, .y = event->y };
        char err = selector(flowbox, pos, npos);
        if (err)
        { D_PRINT("Null mouvement", NULL); return TRUE; }
        gtk_widget_queue_draw(da);
    }

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

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    GMPF_Size size = *GMPF_project_info_get_size(flowbox);
    GMPF_Scale scale = *GMPF_project_info_get_scale(flowbox);

    cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);

    int cur_lay = 0; // Use this variable for debuging
    if (layermngr->layer_list.next != NULL)
    {
        GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        int max_width = size.w * scale.x;
        int max_height = size.h * scale.y;
        gtk_widget_set_size_request(layout, max_width, max_height);
        gtk_widget_set_size_request(da, max_width, max_height);
        gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
        while (lay != NULL)
        {
            cur_lay++;
            if (lay->isvisible)
            {
                cairo_save(cr);
                cairo_scale(cr, scale.x, scale.y);
                cairo_set_source_surface(cr, lay->surface, (double)lay->pos.x, (double)lay->pos.y);
                cairo_paint(cr);
                cairo_restore(cr);
            }
            if (!lay->list.next) break;
            lay = container_of(lay->list.next, GMPF_Layer, list);
        }
    }
    size = *GMPF_selection_get_size(flowbox);
    if (size.w > 0 && size.h > 0)
    {
        GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);
        cairo_save(cr);
        cairo_set_source_rgba(cr, 1, 0, 0, 1);
        cairo_rectangle (cr, pos.x, pos.y, size.w, size.h);
        cairo_stroke(cr);
        cairo_restore(cr);
    }
}


void callback_copy(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    { D_PRINT("No selected layer", NULL); return; }
    GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);
    GMPF_Pos cppos = {.x = (-pos.x+lay->pos.x)/lay->scale_factor.x,
                      .y = (-pos.y+lay->pos.y)/lay->scale_factor.y };

    if (GMPF_selection_copy(flowbox, lay, cppos))
    { PRINTERR("Unable to copy the selection"); return; }
}


void callback_paste(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    { D_PRINT("No selected layer", NULL); return; }
    GMPF_Pos pos = { .x = (layermngr->pos.x - lay->pos.x)/lay->scale_factor.x,
                     .y = (layermngr->pos.y - lay->pos.y)/lay->scale_factor.y};

    if (GMPF_selection_paste(flowbox, lay, pos))
    { PRINTERR("Unable to paste selection"); return; }

    gtk_widget_queue_draw(da);
}


void callback_cut(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    { D_PRINT("No selected layer", NULL); return; }

    GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);
    GMPF_Pos cppos = {.x = (-pos.x+lay->pos.x)/lay->scale_factor.x,
                      .y = (-pos.y+lay->pos.y)/lay->scale_factor.y };

    if (GMPF_selection_cut(flowbox, lay, cppos))
    { PRINTERR("Unable to cut selection"); return; }

    gtk_widget_queue_draw(da);
}


void callback_delete(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
    { D_PRINT("No selected layer", NULL); return; }

    GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);
    GMPF_Pos cppos = {.x = (pos.x-lay->pos.x)/lay->scale_factor.x,
                      .y = (pos.y-lay->pos.y)/lay->scale_factor.y };

    if (GMPF_selection_delete(flowbox, lay, cppos))
    { PRINTERR("Unable to delete selection"); return; }

    gtk_widget_queue_draw(da);
}


/*
 * Calback to select the right tool according to the pressed button in the
 * ToolBar
 */
void callback_select_tool(GtkWidget *widget,
                          UNUSED gpointer   user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    gchar name = gtk_widget_get_name(widget)[0];
    GMPF_Tool tool;
    switch (name)
    {
        case '1': tool = GMPF_TOOL_PAINTER;
                         break;
        case '2': tool = GMPF_TOOL_ERAISER;
                         break;
        case '3': tool = GMPF_TOOL_COLOR_PICKER;
                         break;
        case '4': tool = GMPF_TOOL_COLOR_KILLER;
                         break;
        case '5': tool = GMPF_TOOL_SELECTOR;
                         break;
        case '6': tool = GMPF_TOOL_SELECTOR_FREE;
                         break;
        default : tool = GMPF_TOOL_INCORECT;
        D_PRINT("Unknown tool", NULL);
    }
    layermngr->tool = tool;

    if (tool != GMPF_TOOL_INCORECT && tool != GMPF_TOOL_SELECTOR) // Clear the selection
    {
        GMPF_selection_destroy(flowbox);
        GMPF_selection_init(flowbox);
        gtk_widget_queue_draw(da);
    }
}


void GMPF_init_color_killer_window()
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, button, "ColorKillerApplyButton");
    GET_UI(GtkImage, image, "ColorKillerImage");
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    GError *error = NULL;

    GdkPixbuf *pixbuf = layer ? layer->image : gdk_pixbuf_new_from_file_at_size("images/GMPF_white.png",
                                                         300,
                                                         300,
                                                         &error);
    if (error)
    { PRINTERR ("Unable to load pixbuf"); return; }
    if (layer)
    {
        gtk_widget_show(button);
        float ratio1 = layer->size.w / 300;
        float ratio2 = layer->size.h / 300;
        int finalh = 300;
        int finalw = 300;
        if (ratio1 < ratio2)
            finalw = layer->size.w / ratio2;
        else
            finalh = layer->size.h / ratio1;

        pixbuf = gdk_pixbuf_scale_simple(pixbuf, finalw, finalh,
                             GDK_INTERP_BILINEAR);
    }
    else
    { gtk_widget_hide(button); }

    gtk_image_set_from_pixbuf(image, pixbuf);
}


void GMPF_init_color_swapper_window()
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, button, "ColorSwapperApplyButton");
    GET_UI(GtkImage, image, "ColorSwapperImage");
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    GError *error = NULL;

    GdkPixbuf *pixbuf = layer ? layer->image : gdk_pixbuf_new_from_file_at_size("images/GMPF_white.png",
                                                         300,
                                                         300,
                                                         &error);
    if (error)
    { PRINTERR ("Unable to load pixbuf"); return; }
    if (layer)
    {
        gtk_widget_show(button);
        float ratio1 = layer->size.w / 300;
        float ratio2 = layer->size.h / 300;
        int finalh = 300;
        int finalw = 300;
        if (ratio1 < ratio2)
            finalw = layer->size.w / ratio2;
        else
            finalh = layer->size.h / ratio1;

        pixbuf = gdk_pixbuf_scale_simple(pixbuf, finalw, finalh,
                             GDK_INTERP_BILINEAR);
    }
    else
    { gtk_widget_hide(button); }

    gtk_image_set_from_pixbuf(image, pixbuf);
}


void callback_test_color_swapper(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GMPF_init_color_killer_window();

    GET_UI(GtkImage, image, "ColorSwapperImage");

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);
    GMPF_Layer *layer = layer_initialization();
    layer->surface = gdk_cairo_surface_create_from_pixbuf(pixbuf, 1, NULL);
    GMPF_Size size = { .w = gdk_pixbuf_get_width(pixbuf), .h = gdk_pixbuf_get_height(pixbuf) };

    layer->size = size;
    swap_color(layer);
    gtk_image_set_from_pixbuf(image, layer->image);
    layer_delete(layer);
}


void callback_test_color_killer(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GMPF_init_color_killer_window();

    GET_UI(GtkImage, image, "ColorKillerImage");
    GET_UI(GtkRange, range, "ColorKillerToleranceScale");

    double tolerance = gtk_range_get_value(range) / 100;

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);
    GMPF_Layer *layer = layer_initialization();
    layer->surface = gdk_cairo_surface_create_from_pixbuf(pixbuf, 1, NULL);
    GMPF_Size size = { .w = gdk_pixbuf_get_width(pixbuf), .h = gdk_pixbuf_get_height(pixbuf) };

    layer->size = size;
    kill_color(layer, tolerance);
    gtk_image_set_from_pixbuf(image, layer->image);
    layer_delete(layer);
}


void callback_show_color_killer_window(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkWidget, window, "ColorKillerToleranceWindow");
    GMPF_init_color_killer_window();

    //show the filter creator windowjust
    gtk_widget_show(window);
}


void callback_show_color_swapper_window(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkWidget, window, "ColorSwapperWindow");
    GMPF_init_color_swapper_window();

    //show the filter creator windowjust
    gtk_widget_show(window);
}


void callback_swap_color(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
    swap_color(lay);

    gtk_widget_queue_draw(da);
}


void callback_kill_color(UNUSED GtkWidget *widget, UNUSED gpointer user_data)
{
    GET_UI(GtkRange, scale, "ColorKillerToleranceScale");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);

    double tolerance = gtk_range_get_value(scale) / 100;

    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, lay);
    kill_color(lay, tolerance);

    gtk_widget_queue_draw(da);
}


/*
 * Callback to add a new layer in the list from the "Ajouter un calque" window
 * (Refresh the displayed image of the layer and hide the window)
 */
void callback_add_custom_layer(UNUSED GtkWidget *widget,
                               UNUSED gpointer   user_data)
{
    GET_UI(GtkEntry, name, "LayerNameEntry");
    GET_UI(GtkEntry, width, "LayerWidthSpinButton");
    GET_UI(GtkEntry, height, "LayerHeightSpinButton");
    GET_UI(GtkEntry, offsetX, "LayerOffsetXSpinButton");
    GET_UI(GtkEntry, offsetY, "LayerOffsetYSpinButton");
    GET_UI(GtkWidget, window, "LayerWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    const gchar *n = gtk_entry_get_text(name);
    const gchar *w = gtk_entry_get_text(width);
    const gchar *h = gtk_entry_get_text(height);
    const gchar *x = gtk_entry_get_text(offsetX);
    const gchar *y = gtk_entry_get_text(offsetY);
    char *f = g_object_get_data(G_OBJECT(flowbox), FILENAME_KEY_NAME);
    GMPF_Layer *lay = layermngr_add_new_layer(flowbox, f);
    free(f);
    layer_set_name(lay, (char*)n);
    lay->size.w = atoi(w);
    lay->size.h = atoi(h);
    lay->pos.x = atoi(x);
    lay->pos.y = atoi(y);
    if (!lay->filename)
    {
        if (lay->image)
            g_object_unref(lay->image);
        while (cairo_surface_get_reference_count(lay->surface) > 0)
            cairo_surface_destroy(lay->surface);
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
    GMPF_buffer_add(flowbox, GMPF_ACTION_ADD, lay);

    gtk_widget_hide(window);
}


char GMPF_auto_save_project(GtkFlowBox *flowbox)
{
    char *project_filename = GMPF_project_info_get_filename(flowbox);
    if (check_extension(project_filename, "gmpf"))
    { return 0; }

    char *filename = malloc (sizeof(char) * (2 + strlen(project_filename)));
    sprintf(filename, "%s~", project_filename);
    if (save_project(flowbox, filename))
    {
        PRINTERR("Unable to save project");
        return 1;
    }
    return 0;
}


/*
 * Save the project at the saved filename of the LayerMngr and call the
 * "GMPF_save_under_project()" if it is NULL
 */
gboolean GMPF_save_project()
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    char *project_filename = GMPF_project_info_get_filename(flowbox);
    D_PRINT("pname: %s", project_filename);
    if (!project_filename || check_extension(project_filename, "gmpf"))
        return GMPF_save_under_project();
    else
    {
        char err = save_project(flowbox, (const char*)project_filename);
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
                           UNUSED gpointer     user_data)
{
    GMPF_save_project();
}


/*
 * Save the project at the selected filename
 * (Open a dialog to choose the path where to save the project)
 */
gboolean GMPF_save_under_project()
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");

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
        GMPF_project_info_set_filename(flowbox, filename);
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
                                 UNUSED gpointer     user_data)
{
    GMPF_save_under_project();
}


/*
 * Callback to save the selected layer in a specific file type
 * (Do nothing if there is no selected Layer)
 */
void callback_save_layer(UNUSED GtkMenuItem *menuitem,
                         UNUSED gpointer     user_data)
{
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
                         UNUSED gpointer     user_data)
{
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
    layermngr_clear(flowbox);
    GMPF_Layer *layer = layermngr_add_new_layer(flowbox, filename);
    GMPF_buffer_add(flowbox, GMPF_ACTION_ADD, layer);
    GMPF_project_info_set_filename(flowbox, filename);

    //
    layermngr->image  = gdk_pixbuf_new_from_file(filename, &error);
    int width  = gdk_pixbuf_get_width  (layermngr->image);
    int height = gdk_pixbuf_get_height (layermngr->image);

    char *title = malloc (sizeof(char) * (strlen(filename) + 30));
    sprintf(title, "GMPF - %s : %d * %d", filename, width, height);
    gtk_window_set_title(window, (const char*)title);
    free(title);
    GMPF_saved_state_set_is_saved(flowbox, 0);

    GMPF_Size size = { .w = width, .h = height };
    GMPF_project_info_set_size(flowbox, size);

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
}


void GMPF_init_matrix_window()
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, button, "ApplyMatrixButton");
    GET_UI(GtkImage, image, "FilterImage");
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    GError *error = NULL;
    GdkPixbuf *pixbuf = layer ? layer->image : gdk_pixbuf_new_from_file_at_size("images/GMPF_white.png",
                                                         300,
                                                         300,
                                                         &error);
    if (error)
    { PRINTERR ("Unable to load pixbuf"); return; }
    if (layer)
    {
        gtk_widget_show(button);
        float ratio1 = layer->size.w / 300;
        float ratio2 = layer->size.h / 300;
        int finalh = 300;
        int finalw = 300;
        if (ratio1 < ratio2)
            finalw = layer->size.w / ratio2;
        else
            finalh = layer->size.h / ratio1;

        pixbuf = gdk_pixbuf_scale_simple(pixbuf, finalw, finalh,
                             GDK_INTERP_BILINEAR);
    }
    else
    { gtk_widget_hide(button); }

    gtk_image_set_from_pixbuf(image, pixbuf);
}


/*
 * Show the "Créateur de filtre" window
 * (init all its variables to there default value)
 */
void callback_FC(UNUSED GtkMenuItem *menuitem,
                 UNUSED gpointer     user_data)
{
    GET_UI(GtkWidget, window, "FilterCreator");
    GMPF_init_matrix_window();

    //show the filter creator windowjust
    gtk_widget_show(window);
}


void callback_test_matrix(UNUSED GtkWidget *widget,
                          UNUSED gpointer   user_data)
{
    GMPF_init_matrix_window();
    GET_UI(GtkGrid, grid, "MatrixGrid");
    GET_UI(GtkImage, image, "FilterImage");
    GET_UI(GtkSwitch, sw, "MatrixSizeSwitch");
    size_t mat_size = gtk_switch_get_active(sw) ? 5 : 3;
    double *mat = malloc (sizeof(double) * mat_size * mat_size);
    if (mat_size == 5)
    {
        for (size_t i = 0; i < 25; i++)
        {
            GtkEntry *entry = GTK_ENTRY(gtk_grid_get_child_at(grid, i % 5, i / 5));
            const char *txt = gtk_entry_get_text(entry);
            double res = 0;
            double tmp = 0;
            char op = 0;
            char neg = 1;
            for (size_t j = 0; txt[j] != '\0'; j++)
            {
                if (txt[j] <= '9' && txt[j] >= '0')
                { !op ? (res = res * 10 + txt[j] - '0') : (tmp = tmp * 10 + txt[j] - '0'); }
                else if (txt[j] == '/')
                { op = 1; }
                else if (txt[j] == '-')
                { neg = -1; }
            }
            mat[i] = (op ? res / tmp : res) * neg;

            D_PRINT("mat[%lu, %lu] = %f", i % 5, i / 5, mat[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t k = 0; k < 3; k++)
            {
                GtkEntry *entry = GTK_ENTRY(gtk_grid_get_child_at(grid, (k+1), (i+1)));
                const char *txt = gtk_entry_get_text(entry);
                double res = 0;
                double tmp = 0;
                char op = 0;
                char neg = 1;
                for (size_t j = 0; txt[j] != '\0'; j++)
                {
                    if (txt[j] <= '9' && txt[j] >= '0')
                    { !op ? (res = res * 10 + txt[j] - '0') : (tmp = tmp * 10 + txt[j] - '0'); }
                    else if (txt[j] == '/')
                    { op = 1; }
                    else if (txt[j] == '-')
                    { neg = -1; }
                }
                mat[(i*3)+k] = (op ? res / tmp : res) * neg;

                D_PRINT("mat[%lu, %lu] = %f", (i+1), (k+1), mat[(i*3)+k]);
                }
        }
    }
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);
    GMPF_Layer *layer = layer_initialization();
    layer->surface = gdk_cairo_surface_create_from_pixbuf(pixbuf, 1, NULL);
    GMPF_Size size = { .w = gdk_pixbuf_get_width(pixbuf), .h = gdk_pixbuf_get_height(pixbuf) };
    layer->size = size;
    Convolute(layer, mat, mat_size);
    gtk_image_set_from_pixbuf(image, layer->image);
    layer_delete(layer);
}


/*
 * Show or hide the cases from matrix in the custom filter window according to
 * the selected matrix size_t
 */
void callback_show_hide_matrix_case(GtkSwitch *sw, UNUSED gpointer user_data)
{
    char isOn = gtk_switch_get_active(sw);
    GET_UI(GtkWidget, w0w0w, "0;0");
    GET_UI(GtkWidget, w0w1w, "0;1");
    GET_UI(GtkWidget, w0w2w, "0;2");
    GET_UI(GtkWidget, w0w3w, "0;3");
    GET_UI(GtkWidget, w0w4w, "0;4");
    GET_UI(GtkWidget, w1w0w, "1;0");
    GET_UI(GtkWidget, w1w4w, "1;4");
    GET_UI(GtkWidget, w2w0w, "2;0");
    GET_UI(GtkWidget, w2w4w, "2;4");
    GET_UI(GtkWidget, w3w0w, "3;0");
    GET_UI(GtkWidget, w3w4w, "3;4");
    GET_UI(GtkWidget, w4w0w, "4;0");
    GET_UI(GtkWidget, w4w1w, "4;1");
    GET_UI(GtkWidget, w4w2w, "4;2");
    GET_UI(GtkWidget, w4w3w, "4;3");
    GET_UI(GtkWidget, w4w4w, "4;4");
    isOn ? gtk_widget_show(w0w0w) : gtk_widget_hide(w0w0w);
    isOn ? gtk_widget_show(w0w1w) : gtk_widget_hide(w0w1w);
    isOn ? gtk_widget_show(w0w2w) : gtk_widget_hide(w0w2w);
    isOn ? gtk_widget_show(w0w3w) : gtk_widget_hide(w0w3w);
    isOn ? gtk_widget_show(w0w4w) : gtk_widget_hide(w0w4w);
    isOn ? gtk_widget_show(w1w0w) : gtk_widget_hide(w1w0w);
    isOn ? gtk_widget_show(w1w4w) : gtk_widget_hide(w1w4w);
    isOn ? gtk_widget_show(w2w0w) : gtk_widget_hide(w2w0w);
    isOn ? gtk_widget_show(w2w4w) : gtk_widget_hide(w2w4w);
    isOn ? gtk_widget_show(w3w0w) : gtk_widget_hide(w3w0w);
    isOn ? gtk_widget_show(w3w4w) : gtk_widget_hide(w3w4w);
    isOn ? gtk_widget_show(w4w0w) : gtk_widget_hide(w4w0w);
    isOn ? gtk_widget_show(w4w1w) : gtk_widget_hide(w4w1w);
    isOn ? gtk_widget_show(w4w2w) : gtk_widget_hide(w4w2w);
    isOn ? gtk_widget_show(w4w3w) : gtk_widget_hide(w4w3w);
    isOn ? gtk_widget_show(w4w4w) : gtk_widget_hide(w4w4w);
}


/*
 * Calback to apply "Binarisation" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_binarize(UNUSED GtkMenuItem *menuitem,
                       UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Binarize, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Binarize);
}


/*
 * Calback to apply "Binarisation" filter to all Layer
 */
void callback_binarize_all(UNUSED GtkMenuItem *menuitem,
                           UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Binarize);
    filter_for_selection(Binarize, flowbox);
}


/*
 * Calback to apply "Binarisation Coloré" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_binarize_color(UNUSED GtkMenuItem *menuitem,
                             UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(BinarizeColor, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(BinarizeColor);
}


/*
 * Calback to apply "Binarisation Coloré" filter to the Layer
 */
void callback_binarize_color_all(UNUSED GtkMenuItem *menuitem,
                                 UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(BinarizeColor);
    filter_for_selection(BinarizeColor, flowbox);
}


/*
 * Calback to apply the correct convolution matrix according to the pressed
 * button to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_convolute_f(UNUSED GtkMenuItem *menuitem,
                          UNUSED gpointer     user_data)
{

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
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, layer);
    Convolute(layer, mat, 3);
}


/*
 * Calback to apply "Niveau de gris" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_grey(UNUSED GtkMenuItem *menuitem,
                   UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Greyscale, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Greyscale);
}


/*
 * Calback to apply "Niveau de gris" filter to all Layer
 */
void callback_grey_all(UNUSED GtkMenuItem *menuitem,
                       UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Greyscale);
    filter_for_selection(Greyscale, flowbox);
    gtk_widget_queue_draw(da);
}


/*
 * Calback to apply "Teinture" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_tinter(UNUSED GtkMenuItem *menuitem,
                UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Tinter, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Tinter);
}


void callback_tinter_all(UNUSED GtkMenuItem *menuitem,
                         UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Tinter);
    filter_for_selection(Tinter, flowbox);
    gtk_widget_queue_draw(da);
}


/*
 * Calback to apply "Accentuation des couleurs" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_colorfull(UNUSED GtkMenuItem *menuitem,
                        UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Colorfull, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Colorfull);
}


void callback_colorfull_all(UNUSED GtkMenuItem *menuitem,
                            UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Colorfull);
    filter_for_selection(Colorfull, flowbox);
    gtk_widget_queue_draw(da);
}


/*
 * Calback to apply "Négatif" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_negative(UNUSED GtkMenuItem *menuitem,
                       UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Negative, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Negative);
}


/*
 * Calback to apply "Négatif" filter to the all Layer
 */
void callback_negative_all(UNUSED GtkMenuItem *menuitem,
                           UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Negative);
    filter_for_selection(Negative, flowbox);
}


/*
 * Calback to apply "Luminosité-" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_darkness(UNUSED GtkMenuItem *menuitem,
                       UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Darkness, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Darkness);
}


/*
 * Calback to apply "Luminosité-" filter to all Layer
 */
void callback_darkness_all(UNUSED GtkMenuItem *menuitem,
                           UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Darkness);
    filter_for_selection(Darkness, flowbox);
}


/*
 * Calback to apply "Luminosité+" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_lightness(UNUSED GtkMenuItem *menuitem,
                        UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Lightness, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Lightness);
}


/*
 * Calback to apply "Luminosité+" filter to all Layer
 */
void callback_lightness_all(UNUSED GtkMenuItem *menuitem,
                            UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Lightness);
    filter_for_selection(Lightness, flowbox);
}


/*
 * Calback to apply "Egalisation" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_equalize(UNUSED GtkMenuItem *menuitem,
                       UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Equalize, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Equalize);
}


/*
 * Calback to apply "Egalisation" filter to all Layer
 */
void callback_equalize_all(UNUSED GtkMenuItem *menuitem,
                           UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Equalize);
    filter_for_selection(Equalize, flowbox);
}


/*
 * Calback to apply "Egalisation Coloré" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_equalize_color(UNUSED GtkMenuItem *menuitem,
                             UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Equalize_color, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Equalize_color);
}


/*
 * Calback to apply "Egalisation Coloré" filter to all Layer
 */
void callback_equalize_color_all(UNUSED GtkMenuItem *menuitem,
                                 UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Equalize_color);
    filter_for_selection(Equalize_color, flowbox);
}


/*
 * Calback to apply "Mirroir Horizontal" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_horizontale(UNUSED GtkMenuItem *menuitem,
                          UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Horizontale, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Horizontale);
}


/*
 * Calback to apply "Mirroir Horizontal" filter to all Layer
 */
void callback_horizontale_all(UNUSED GtkMenuItem *menuitem,
                              UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Horizontale);
    filter_for_selection(Horizontale, flowbox);
}


/*
 * Calback to apply "Mirroir Vertical" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_verticale(UNUSED GtkMenuItem *menuitem,
                        UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection(Verticale, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer(Verticale);
}


/*
 * Calback to apply "Mirroir Vertical" filter to all Layer
 */
void callback_verticale_all(UNUSED GtkMenuItem *menuitem,
                            UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer(Verticale);
    filter_for_selection(Verticale, flowbox);
}


void reset_cursor(UNUSED GtkMenuItem *menuitem)
{
    GET_UI(GtkWindow, window, "MainWindow");
    resetCursor(window);
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr->tool = GMPF_TOOL_INCORECT;
}


/*
 * Callback to save the newly created filter using "Créatteur de Filtre" window
 * (Do noting if no filter has been selected)
 */
void callback_saveFilter(UNUSED GtkButton *btn,
                         UNUSED gpointer   user_data)
{
    Save_filter();
    GET_UI(GtkWidget, parent, "FilterCreator");
    gtk_widget_hide(parent);
}


/*
 * Calback to apply "Chauf" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_warm(UNUSED GtkMenuItem *menuitem,
                   UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection_color(Color_balance, 100, 255, 255, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer_color(Color_balance, 100, 255, 255);
}


/*
 * Calback to apply "chaud" filter to all Layer
 */
void callback_warm_all(UNUSED GtkMenuItem *menuitem,
                       UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer_color(Color_balance, 100, 255, 255);
        filter_for_selection_color(Color_balance, 100, 255, 255, flowbox);
}


/*
 * Calback to apply "Vert" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_green(UNUSED GtkMenuItem *menuitem,
                    UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection_color(Color_balance, 255, 100, 255, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer_color(Color_balance, 255, 100, 255);
}


/*
 * Calback to apply "Vert" filter to all Layer
 */
void callback_green_all(UNUSED GtkMenuItem *menuitem,
                        UNUSED gpointer     user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_filter_apply_to_all_layer_color(Color_balance, 255, 100, 255);
        filter_for_selection_color(Color_balance, 255, 100, 255, flowbox);
}


/*
 * Calback to apply "Froid" filter to the selected Layer
 * (Do nothing if there is no selected Layer)
 */
void callback_cold(UNUSED GtkMenuItem *menuitem,
                   UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr->tool == GMPF_TOOL_SELECTOR)
    {
        filter_for_selection_color(Color_balance, 255, 255, 100, flowbox);
        gtk_widget_queue_draw(da);
    }
    else
        GMPF_filter_apply_to_selected_layer_color(Color_balance, 255, 255, 100);
}


/*
 * Calback to apply "Froid" filter to all Layer
 */
void callback_cold_all(UNUSED GtkMenuItem *menuitem,
                       UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
	GMPF_filter_apply_to_all_layer_color(Color_balance, 255, 255, 100);
    filter_for_selection_color(Color_balance, 255, 255, 100, flowbox);
}


/*
 * Callback to apply the selected *.ftr file filter to the selcted Layer
 * (Do nothing if there is no selected Layer)
 * (Open a dialog to select the *.ftr file)
 */
void callback_applyFilter(UNUSED GtkWidget *btn,
                          UNUSED gpointer   user_data)
{
    //a modifier -> recuprer le fichier du dialog
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkGrid, grid, "MatrixGrid");
    GET_UI(GtkImage, image, "FilterImage");
    GET_UI(GtkSwitch, sw, "MatrixSizeSwitch");

    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);

    size_t mat_size = gtk_switch_get_active(sw) ? 5 : 3;
    double *mat = malloc (sizeof(double) * mat_size * mat_size);
    if (mat_size == 5)
    {
        for (size_t i = 0; i < 25; i++)
        {
            GtkEntry *entry = GTK_ENTRY(gtk_grid_get_child_at(grid, i % 5, i / 5));
            const char *txt = gtk_entry_get_text(entry);
            double res = 0;
            double tmp = 0;
            char op = 0;
            char neg = 1;
            for (size_t j = 0; txt[j] != '\0'; j++)
            {
                if (txt[j] <= '9' && txt[j] >= '0')
                { !op ? (res = res * 10 + txt[j] - '0') : (tmp = tmp * 10 + txt[j] - '0'); }
                else if (txt[j] == '/')
                { op = 1; }
                else if (txt[j] == '-')
                { neg = -1; }
            }
            mat[i] = (op ? res / tmp : res) * neg;

            D_PRINT("mat[%lu, %lu] = %f", i % 5, i / 5, mat[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t k = 0; k < 3; k++)
            {
                GtkEntry *entry = GTK_ENTRY(gtk_grid_get_child_at(grid, (k+1), (i+1)));
                const char *txt = gtk_entry_get_text(entry);
                double res = 0;
                double tmp = 0;
                char op = 0;
                char neg = 1;
                for (size_t j = 0; txt[j] != '\0'; j++)
                {
                    if (txt[j] <= '9' && txt[j] >= '0')
                    { !op ? (res = res * 10 + txt[j] - '0') : (tmp = tmp * 10 + txt[j] - '0'); }
                    else if (txt[j] == '/')
                    { op = 1; }
                    else if (txt[j] == '-')
                    { neg = -1; }
                }
                mat[(i*3)+k] = (op ? res / tmp : res) * neg;

                D_PRINT("mat[%lu, %lu] = %f", (i+1), (k+1), mat[(i*3)+k]);
                }
        }
    }
    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, layer);
    Convolute(layer, mat, mat_size);
}


void callback_undo(UNUSED GtkWidget *widget,
                   UNUSED gpointer   user_data)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    GMPF_Layer *lay = NULL;
    if (layermngr->layer_list.next != NULL)
        lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);

    GMPF_Size size = *GMPF_project_info_get_size(flowbox);
    // if (!size) { return; }
    GMPF_Scale scale = *GMPF_project_info_get_scale(flowbox);
    // if (!scale) { return; }

    int max_width = size.w * scale.x;
    int max_height = size.h * scale.y;

    GMPF_buffer_undo(flowbox);

    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
    gtk_widget_queue_draw(da);
}


void callback_redo(UNUSED GtkWidget *widget,
                   UNUSED gpointer   user_data)
{
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    GMPF_Size size = *GMPF_project_info_get_size(flowbox);
    // if (!size) { return; }
    GMPF_Scale scale = *GMPF_project_info_get_scale(flowbox);
    // if (!scale) { return; }

    int max_width = size.w * scale.x;
    int max_height = size.h * scale.y;

    GMPF_buffer_redo(flowbox);

    gtk_widget_set_size_request(layout, max_width, max_height);
    gtk_widget_set_size_request(da, max_width, max_height);
    gtk_layout_set_size((GtkLayout *)layout, max_width, max_height);
    gtk_widget_queue_draw(da);
}


/*
 * Callback to quit the application
 * (Ask confirmation before quit)
 */
void callback_quit(UNUSED GtkWidget *widget,
                   UNUSED gpointer   user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");
    int confirm = 2;

    if (!GMPF_saved_state_get_is_saved(flowbox))
        confirm = open_confirm_quit_without_saving_dialog(NULL);

    if (confirm == 0)
        return;

    else if (confirm == 1)
        if (!GMPF_save_project())
            return;

    GMPF_quit(flowbox, window);
}


/*
 * Callback ask the user if he want to quit the application with the cross
 * icon on the top right corner of the window
 * (Ask confirmation before quit)
 */
gboolean do_destroy_event(UNUSED GtkWidget *widget,
                          UNUSED GdkEvent  *event,
                          UNUSED gpointer   user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWindow, window, "MainWindow");
    int confirm = 2;
    if (!GMPF_saved_state_get_is_saved(flowbox))
        confirm = open_confirm_quit_without_saving_dialog(NULL);

    if (confirm == 0)
        return TRUE;

    else if (confirm == 1)
        if (!GMPF_save_project())
            return TRUE;

    GMPF_quit(flowbox, window);
    return FALSE;
}


/*
 * Callback to edit selected layer's properties
 */
void callback_edit_layer_properties(UNUSED GtkWidget *widget,
                                    UNUSED gpointer   user_data)
{
    GET_UI(GtkEntry, name, "LayerNameEntry1");
    GET_UI(GtkEntry, width, "LayerWidthSpinButton1");
    GET_UI(GtkEntry, height, "LayerHeightSpinButton1");
    GET_UI(GtkEntry, offsetX, "LayerOffsetXSpinButton1");
    GET_UI(GtkEntry, offsetY, "LayerOffsetYSpinButton1");
    GET_UI(GtkWidget, window, "EditLayerWindow");
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");

    const gchar *n = gtk_entry_get_text(name);
    const gchar *w = gtk_entry_get_text(width);
    const gchar *h = gtk_entry_get_text(height);
    const gchar *x = gtk_entry_get_text(offsetX);
    const gchar *y = gtk_entry_get_text(offsetY);

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    GMPF_buffer_add(flowbox, GMPF_ACTION_CHANGE_PROPERTIES, lay);
    layer_set_name(lay, (char*)n);
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
                                                UNUSED gpointer   user_data)
{
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

    gtk_entry_set_text(name, lay->name);
    gtk_spin_button_set_value(width, lay->size.w);
    gtk_spin_button_set_value(height, lay->size.h);
    gtk_spin_button_set_value(offsetX, lay->pos.x);
    gtk_spin_button_set_value(offsetY, lay->pos.y);

    gtk_widget_show(layer_window);
}

/*
 * Callback to clear the LayerMngr and reset it
 */
void callback_clear_GMPF_LayerMngr(UNUSED GtkMenuItem *menuitem,
                                   UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    layermngr_clear(flowbox);
}


/*
 * Callback to remove the selected Layer from the Layer list
 * (Do nothing if there is no selected Layer)
 */
void callback_remove_selected_layer(UNUSED GtkMenuItem *menuitem,
                                    UNUSED gpointer     user_data)
{
    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GET_UI(GtkWidget, da, "drawingArea");
    GET_UI(GtkWidget, layout, "DrawingAreaLayout");
    if (!layermngr_get_selected_layer(flowbox))
    { return; }

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    int confirm = 2;
    if (layermngr->layer_list.next && !layermngr->layer_list.next->next)
    {
        const char *msg = "Supprimer ce calque ferme le projet.\nVoulez vous continuer ?";
        confirm = open_confirm_quit_without_saving_dialog(msg);
    }
    if (!confirm) { return; }
    if (confirm == 1) { GMPF_save_project(); }

    layermngr_delete_selected_layer(flowbox);
    if (!layermngr->layer_list.next)
    {
        layermngr_clear(flowbox);
        GMPF_buffer_reset(flowbox);
        GMPF_project_info_reset(flowbox);
        gtk_widget_set_size_request(layout, 0, 0);
        gtk_widget_set_size_request(da, 0, 0);
        gtk_layout_set_size((GtkLayout *)layout, 0, 0);
    }
    gtk_widget_queue_draw(da);
}


void callback_select_brush(GtkWidget *brush, UNUSED gpointer user_data)
{

    GET_UI(GtkFlowBox, flowbox, "GMPF_flowbox");
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (strcmp(gtk_widget_get_name(brush), "Circulaire") == 0)
        layermngr->brush = 0;

    if (strcmp(gtk_widget_get_name(brush), "Carré") == 0)
        layermngr->brush = 1;

    if (strcmp(gtk_widget_get_name(brush), "Rectangle") == 0)
        layermngr->brush = 2;

    if (strcmp(gtk_widget_get_name(brush), "Triangle") == 0)
        layermngr->brush = 3;

    D_PRINT("brush: %s", gtk_widget_get_name(brush));
}


void callback_show_window(UNUSED GtkMenuItem *menuitem, GtkWindow *window)
{
    gtk_widget_show(GTK_WIDGET(window));
}
