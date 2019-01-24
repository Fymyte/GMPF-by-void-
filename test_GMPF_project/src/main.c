#include <gtk-3.0/gtk/gtk.h>
#include <err.h>

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;

void callback_image(GtkFileChooser *filebtn, gpointer user_data);
void callback_about (GtkMenuItem *menuitem, gpointer user_data);
void callback_adjust_scale (GtkRange *scale, gpointer user_data);
//static gboolean key_event(GtkWidget *widget, GdkEventKey *event);

struct _GdkPixbuf *imgPixbuf = NULL;

int
main(int argc, char *argv [])
{
    GtkWidget *Main_window = NULL;
    SGlobalData data;  /* variable propagée à tous les callbacks. */
    gchar *filename = NULL;
    GError *error = NULL;

    /* Initialisation de la librairie Gtk. */
    gtk_init(&argc, &argv);

    /* Ouverture du fichier Glade de la fenêtre principale */
    data.builder = gtk_builder_new();

    /* Création du chemin complet pour accéder au fichier test.glade. */
    /* g_build_filename(); construit le chemin complet en fonction du système */
    /* d'exploitation. ( / pour Linux et \ pour Windows) */
    filename =  g_build_filename ("test.glade", NULL);

    /* Chargement du fichier test.glade. */
    gtk_builder_add_from_file (data.builder, filename, &error);
    g_free (filename);
    if (error)
    {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
    }


    /* Affectation des signaux de l'interface aux différents CallBacks. */
    gtk_builder_connect_signals (data.builder, &data);

    /* Récupération du pointeur de la fenêtre principale */
    Main_window = GTK_WIDGET(gtk_builder_get_object (data.builder, "MainWindow"));
    //g_signal_connect(Main_window, "key-release-event", G_CALLBACK(key_event), NULL);

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all (Main_window);

    gtk_main();

    return 0;
}

void callback_about (GtkMenuItem *menuitem, gpointer user_data)
{
    /* Transtypage du pointeur user_data pour récupérer nos données. */
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *dialog = NULL;

    dialog =  gtk_about_dialog_new ();

    /* Pour l'exemple on va rendre la fenêtre "À propos" modale par rapport à la */
    /* fenêtre principale. */
    gtk_window_set_transient_for (GTK_WINDOW(dialog), GTK_WINDOW(gtk_builder_get_object (data->builder, "MainWindow")));

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    menuitem = 0;
}

// static gboolean key_event(GtkWidget *widget, GdkEventKey *event)
// {
//     char *key = gdk_keyval_name (event->keyval);
//     if (event->keyval == 65362)
//     {
//         callback_adjust_sacle
//     }
//     g_printerr("%d : %s\n", event->keyval, key);
//     return FALSE;
// }


void callback_adjust_scale(GtkRange *scale, gpointer user_data)
{

    SGlobalData *data = (SGlobalData*) user_data;

    // if (scale == NULL)
    // {
    //     scale = gtk_builder_get_object(data->builder, "Scaler");
    // }

    GtkImage *image = NULL;

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    // GError *err = NULL;
    // struct _GdkPixbuf *imgPixbuf;
    // imgPixbuf = gtk_image_get_pixbuf (image);

    // if(err)
    // {
    //     g_print("Error here : %s\n", err->message);
    //     g_error_free(err);
    // }

    //gtk_image_clear(image);

    gdouble scaleValue = gtk_range_get_value (scale);

    int imgwidth = gdk_pixbuf_get_width(imgPixbuf) * scaleValue;
    int imgheight = gdk_pixbuf_get_height(imgPixbuf) * scaleValue;

    struct _GdkPixbuf *img2 = gdk_pixbuf_scale_simple(imgPixbuf, imgwidth, imgheight, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(image, img2);
}

void callback_image(GtkFileChooser *filebtn, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    //create the variable wich will contain the file path
    char *filename = NULL;
    GtkImage *image = NULL;

    //get the path from the gtk file chooser
    filename = gtk_file_chooser_get_filename(filebtn);

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    //print the image on the gtk image box
    gtk_image_clear(image);
    //gtk_image_set_from_file(image, filename);

    //get the pixbuf from the gtk image
    GError *err = NULL;
    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gdk_pixbuf_new_from_file(filename, &err);

    if(err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
    }

    int imgwidth = gdk_pixbuf_get_width(imgPixbuf) / 4;
    int imgheight = gdk_pixbuf_get_height(imgPixbuf) / 4;

    //change the size of the pixbuf
    struct _GdkPixbuf *img2 = gdk_pixbuf_scale_simple(imgPixbuf, imgwidth, imgheight, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(image, img2);
}