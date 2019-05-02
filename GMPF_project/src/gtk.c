#include "gtk.h"

/*
 * Init all variables and UI element of the application and launch it
 */
int GMPF_start()
{
    GtkWidget *Main_window = NULL;
    SGlobalData data;  /* variable propagée à tous les callbacks. */
    gchar *filename = NULL;
    GError *error = NULL;

    /* Ouverture du fichier Glade de la fenêtre principale */
    data.builder = gtk_builder_new();

    /* Création du chemin complet pour accéder au fichier test.glade. */
    /* g_build_filename(); construit le chemin complet en fonction du système */
    /* d'exploitation. ( / pour Linux et \ pour Windows) */
    filename =  g_build_filename (INTERFACE_NAME, NULL);

    /* Chargement du fichier test.glade. */
    gtk_builder_add_from_file (data.builder, filename, &error);
    g_free (filename);
    if (error)
    {
        //gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return 1;
    }


    /* Affectation des signaux de l'interface aux différents CallBacks. */
    gtk_builder_connect_signals (data.builder, &data);
    GError *err = NULL;
    // unchangedPixbuf = gdk_pixbuf_new_from_file("gimp_logo.png", &err);
    if (err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
    }

    /* Récupération du pointeur de la fenêtre principale */
    Main_window = GTK_WIDGET(gtk_builder_get_object (data.builder, "MainWindow"));
    //g_signal_connect(Main_window, "key-release-event", G_CALLBACK(key_event), NULL);

    // resetCursor(&data);



    // INIT THE WINDOW ELEMENT
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data.builder, "GMPF_flowbox"));
    layermngr_create(flowbox);
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    layermngr->brush_size = 4;

    GMPF_saved_state_init(flowbox);
    GMPF_selection_init(flowbox);
    GMPF_buffer_init(flowbox);

    GtkWidget *da = NULL;
    da = GTK_WIDGET(gtk_builder_get_object(data.builder, "drawingArea"));
    gtk_widget_set_events (da, gtk_widget_get_events (da)
                                | GDK_BUTTON_PRESS_MASK
                                | GDK_POINTER_MOTION_MASK
                                | GDK_BUTTON_RELEASE_MASK
                                | GDK_ENTER_NOTIFY_MASK
                                | GDK_LEAVE_NOTIFY_MASK);

    /* Use this to solve problems*/
    // g_free(gdk_window);

    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *myFile = CSSTHEME;

    GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(gtk_builder_get_object(data.builder, "MainWindow")));
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myFile), &err);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider),
                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);

    gtk_widget_hide_on_delete(Main_window);
    gtk_widget_show_all (Main_window);

/************************Uncompress the event from the drawing area************/
    GdkWindow *gdk_window = gtk_widget_get_window(da);
    gdk_window_set_event_compression (gdk_window, FALSE);

    gtk_main();

    return 0;
}


/*
 * Quit the application, free all element and close all window
 */
void GMPF_quit (GtkFlowBox *flowbox,
                GtkWindow  *window)
{
    layermngr_delete(flowbox);
    GMPF_saved_state_destroy(flowbox);
    GMPF_selection_destroy(flowbox);
    GMPF_buffer_destroy(flowbox);
    resetCursor(window);
    gtk_main_quit();
}
