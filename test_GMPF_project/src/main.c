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
void callback_grey(GtkMenuItem *menuitem, gpointer user_data);
void callback_binarize(GtkMenuItem *menuitem, gpointer user_data);

//pixels operations functions
void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha);
gboolean gdkpixbuf_get_colors_by_coordinates(GdkPixbuf *pixbuf, gint x, gint     y, guchar *red, guchar *green, guchar *blue, guchar *alpha);

//static gboolean key_event(GtkWidget *widget, GdkEventKey *event);

struct _GdkPixbuf *imgPixbuf;

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

    //struct _GdkPixbuf *imgPixbuf = NULL;
    //imgPixbuf = gtk_image_get_pixbuf (image);

    //gtk_image_clear(image);

    gdouble scaleValue = gtk_range_get_value (scale);
    if (scaleValue < 0)
    {
        scaleValue = -scaleValue / 100;
    }

    g_print("scaleValue = %f", scaleValue);

    int imgwidth = gdk_pixbuf_get_width(imgPixbuf) * scaleValue;
    int imgheight = gdk_pixbuf_get_height(imgPixbuf) * scaleValue;

    struct _GdkPixbuf *img2 = gdk_pixbuf_scale_simple(imgPixbuf, imgwidth, imgheight, GDK_INTERP_BILINEAR);
    imgPixbuf = img2;

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
    //struct _GdkPixbuf *imgPixbuf;
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
    imgPixbuf = img2;


    gtk_image_set_from_pixbuf(image, img2);
}

void callback_grey(GtkMenuItem *menuitem, gpointer user_data)
{
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image= GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));
    
    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

    guchar red;
    guchar green;
    guchar blue, alpha;

    //gdkpixbuf_get_colors_by_coordinates(imgPixbuf, 0, 0, red, green, blue);
    guchar grey;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, j, i, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");
            grey = (red + green + blue) / 3;
            put_pixel(imgPixbuf, j, i, grey, grey, grey, alpha);
        }
    }
}

void callback_binarize(GtkMenuItem *menuitem, gpointer user_data)
{
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image= GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"))    ;   

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

    guchar red;
    guchar green;
    guchar blue, alpha;


    guchar grey;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);
    gboolean error = FALSE;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, j, i, &red, &green, &blue, &alpha);
            if(!error)
                err(1, "pixbuf get pixels error");
            grey = (red + green + blue) / 3;
            if(grey <= 127)
                put_pixel(imgPixbuf, j, i, 0, 0, 0, alpha);
            else
                put_pixel(imgPixbuf, j, i, 255, 255, 255, alpha);
        }
    }
}

void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
{
    int width, height, rowstride, n_channels;
    guchar *pixels, *p;

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);

    g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    g_assert (n_channels == 4);

    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);

    g_assert (x >= 0 && x < width);
    g_assert (y >= 0 && y < height);

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * n_channels;
    p[0] = red;
    p[1] = green;
    p[2] = blue;
    p[3] = alpha;
}

gboolean gdkpixbuf_get_colors_by_coordinates(GdkPixbuf *pixbuf, gint x, gint y, guchar *red, guchar *green, guchar *blue, guchar *alpha)
{
    guchar *pixel=NULL;
    gint channel=0;
    gint width=0;

    if (pixbuf == NULL) return FALSE;
    if (x<0 || y<0) return FALSE;
    if (x>=gdk_pixbuf_get_width(pixbuf)) return FALSE;
    if (y>=gdk_pixbuf_get_height(pixbuf)) return FALSE;

    pixel=gdk_pixbuf_get_pixels(pixbuf);
    channel=gdk_pixbuf_get_n_channels(pixbuf);
    width=gdk_pixbuf_get_width(pixbuf);

    *red   = pixel[(x*channel)+(y*width*channel)];
    *green = pixel[(x*channel)+(y*width*channel)+1];
    *blue  = pixel[(x*channel)+(y*width*channel)+2];
    *alpha = pixel[(x*channel)+(y*width*channel)+3];

    return TRUE;
}
