#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "Struct.h"
#include "Matrix.h"
#include "cursor.h"

#include "GMPF_LayerMngr.h"

/*typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;
*/

//callback functions
void callback_image(GtkFileChooser *filebtn, gpointer user_data);
void callback_about (GtkMenuItem *menuitem, gpointer user_data);
void callback_adjust_scale (GtkEntry *entry, gpointer user_data);
void callback_grey(GtkMenuItem *menuitem, gpointer user_data);
void callback_binarize(GtkMenuItem *menuitem, gpointer user_data);
void callback_FC(GtkMenuItem *menuitem, gpointer user_data);
void callback_hideWidget(GtkWidget *widget, gpointer user_data);
void callback_setCursor(GtkMenuItem *menuitem, gpointer user_data);

    // GMPF_LayerMngr
void callback_clear_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);
void callback_add_GMPF_Layer(GtkMenuItem *menuitem, gpointer user_data);
void callback_remove_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);



//pixels operations functions
void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green,
                            guchar blue, guchar alpha);
gboolean gdkpixbuf_get_colors_by_coordinates(GdkPixbuf *pixbuf, gint x,
                            gint y, guchar *red, guchar *green, guchar *blue,
                            guchar *alpha);

//Other functions
void GMPFquit(GtkMenuItem *menuitem, gpointer user_data);

//static gboolean key_event(GtkWidget *widget, GdkEventKey *event);

struct _GdkPixbuf *imgPixbuf;
struct _GdkPixbuf *unchangedPixbuf;
const char *interface_file = "interface.glade";

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
    filename =  g_build_filename (interface_file, NULL);

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
    unchangedPixbuf = gdk_pixbuf_new_from_file("gimp_logo.png", &err);
    if (err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
    }

    /* Récupération du pointeur de la fenêtre principale */
    Main_window = GTK_WIDGET(gtk_builder_get_object (data.builder, "MainWindow"));
    //g_signal_connect(Main_window, "key-release-event", G_CALLBACK(key_event), NULL);

    resetCursor(&data);



    // INIT THE WINDOW ELEMENT
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data.builder, "GMPF_flowbox"));
    layermngr_create(flowbox);




    gtk_widget_show_all (Main_window);

    gtk_main();

    return 0;
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

// void callback_create_subPixbuf(GTKRange *range, gpointer user_data)
// {
//     SGlobalData *data = (SGlobalData*) user_data;
//
//     GtkImage *image = NULL;
//     struct
//}

/** Define a struct which is similar to an image **/
struct Img_rgb *init_img_rgb(int rows, int cols)
{
    struct Img_rgb *img = malloc(sizeof(struct Img_rgb));
    img -> rows = rows;
    img -> cols = cols;
    img -> red = init_matrix(rows, cols);
    img -> green = init_matrix(rows, cols);
    img -> blue = init_matrix(rows, cols);
    img -> alpha = init_matrix(rows, cols);
    return img;
}

void img_rgb_zero(struct Img_rgb *img)
{
    InitializeMatrixZero(img -> red);
    InitializeMatrixZero(img -> green);
    InitializeMatrixZero(img -> blue);
}

void free_img_rgb(struct Img_rgb *img)
{
    FreeMatrix(img -> red);
    FreeMatrix(img -> green);
    FreeMatrix(img -> blue);
    free(img);
}

void Img_rgb_to_Image(struct _GdkPixbuf *imgPixbuf, struct Img_rgb *img)
{
    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);

    guchar r;
    guchar g;
    guchar b;
    guchar a;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            r = Matrix_IJ(img -> red, i, j);
            g = Matrix_IJ(img -> green, i , j);
            b = Matrix_IJ(img -> blue, i, j);
            a = Matrix_IJ(img -> alpha, i, j);

            put_pixel(imgPixbuf, i, j, r, g, b, a);
        }
    }
}

int check(int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i > width || j > height)
        return 0;
    return 1;
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

gboolean gdkpixbuf_get_colors_by_coordinates(GdkPixbuf *pixbuf, gint x, gint y
    , guchar *red, guchar *green, guchar *blue, guchar *alpha)
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

void callback_hideWidget(GtkWidget *widget, gpointer user_data)
{
    gtk_widget_hide(widget);
}

void GMPFquit(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    resetCursor(data);
    gtk_main_quit();
}


















    // GMPF_LayerMngr
void callback_clear_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr_clear(flowbox);
}

void callback_add_GMPF_Layer(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr_add_new_layer(flowbox);
}

void callback_remove_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr_delete_selected_layer(flowbox);
}
