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
void callback_create_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);
void callback_clear_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);
void callback_add_GMPF_Layer(GtkMenuItem *menuitem, gpointer user_data);
void callback_remove_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);



//pixels operations functions
void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha);
gboolean gdkpixbuf_get_colors_by_coordinates(GdkPixbuf *pixbuf, gint x, gint y, guchar *red, guchar *green, guchar *blue, guchar *alpha);

//Other functions
void GMPFquit(GtkMenuItem *menuitem, gpointer user_data);

//static gboolean key_event(GtkWidget *widget, GdkEventKey *event);

struct _GdkPixbuf *imgPixbuf;
struct _GdkPixbuf *unchangedPixbuf;
int default_size_width = 0;
int default_size_height = 0;
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

// void callback_create_subPixbuf(GTKRange *range, gpointer user_data)
// {
//     SGlobalData *data = (SGlobalData*) user_data;
//
//     GtkImage *image = NULL;
//     struct
//}


void callback_adjust_scale(GtkEntry *entry, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkImage *image = NULL;

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf = NULL;
    if (!unchangedPixbuf)
        imgPixbuf = gtk_image_get_pixbuf (image);
    else
        imgPixbuf = unchangedPixbuf;
    //gtk_image_clear(image);
    gchar *s = gtk_entry_get_text (entry);
    int scaleValue = atoi(s);

    scaleValue = scaleValue / 100;

    if (default_size_width == 0 || default_size_height == 0)
    {
        default_size_width = gdk_pixbuf_get_width(imgPixbuf);
        default_size_height = gdk_pixbuf_get_height(imgPixbuf);
    }
    int imgwidth = default_size_width * scaleValue;
    int imgheight = default_size_height * scaleValue;

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
    g_print("filename : %s\n", filename);

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    //print the image on the gtk image box
    gtk_image_clear(image);
    //gtk_image_set_from_file(image, filename);

    //get the pixbuf from the gtk image
    GError *err = NULL;
    //struct _GdkPixbuf *imgPixbuf;
    unchangedPixbuf = gdk_pixbuf_new_from_file(filename, &err);

    if(err)
    {
        printf("Error : %s\n", err->message);
        g_error_free(err);
    }

    int default_size_width = gdk_pixbuf_get_width(unchangedPixbuf) / 4;
    int default_size_height = gdk_pixbuf_get_height(unchangedPixbuf) / 4;

    //change the size of the pixbuf
    struct _GdkPixbuf *img2 = gdk_pixbuf_scale_simple(unchangedPixbuf, default_size_width, default_size_height, GDK_INTERP_BILINEAR);
    unchangedPixbuf = img2;

    g_print("def_width = %d; def_height = %d", default_size_width, default_size_height);


    gtk_image_set_from_pixbuf(image, img2);
}
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


void callback_binarize(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Binarize\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image= GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

    guchar red, green, blue, alpha;

    guchar grey;

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
            grey = (red + green + blue) / 3;
            if (grey <= 127)
                put_pixel(imgPixbuf, i, j, 0, 0, 0, alpha);
            else
                put_pixel(imgPixbuf, i, j, 255, 255, 255, alpha);
        }
    }
    //struct _GtkPixbuf *img2 = imgPixbuf;
    //gtk_image_clear(image);
    gtk_image_set_from_pixbuf(image, imgPixbuf);
    // gtk_widget_queue_draw(image);
}

void callback_binarize_color(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Binarize color\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image= GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

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
            if (red > 127)
                red = 255;
            else
                red = 0;

            if (green > 127)
                green = 255;
            else
                green = 0;

            if (blue > 127)
                blue = 255;
            else
                blue = 0;

            put_pixel(imgPixbuf, i, j, red, green, blue, alpha);
        }
    }
    gtk_image_set_from_pixbuf(image, imgPixbuf);
}

int check(int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i > width || j > height)
        return 0;
    return 1;
}

void callback_convolute_f(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Convolution\n");
    int number;
    printf("Waiting for a number....\n");
    printf("0 : Edge Enhance\n");
    printf("1 : Sharpen\n");
    printf("2 : Blur\n");
    if (scanf("%i", &number) == EOF)
        errx(1, "Error, scanf()");

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
    image= GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

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
                        error= gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
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

    gtk_image_set_from_pixbuf(image, imgPixbuf);

    free_img_rgb(img);
    free(mat);
}


void callback_grey(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Grey\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image= GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

    guchar red, green, blue, alpha;

    guchar grey;

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
            grey = (red + green + blue) / 3;
            put_pixel(imgPixbuf, i, j, grey, grey, grey, alpha);
        }
    }
    gtk_image_set_from_pixbuf(image, imgPixbuf);
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

void callback_negative(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Negative\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

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
    gtk_image_set_from_pixbuf(image, imgPixbuf);
}

void callback_tinter(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Tinter\n");

    guchar r, g, b;
    int factor;

    printf("Waiting for colors values ...\n");
    printf("Enter red value\n");
    if (scanf("%hhu", &r) == EOF)
        errx(1, "Error, scanf()");

    printf("Enter green value\n");
    if (scanf("%hhu", &g) == EOF)
        errx(1, "Error, scanf()");

    printf("Enter blue value\n");
    if (scanf("%hhu", &b) == EOF)
        errx(1, "Error, scanf()");

    printf("Enter factor (between 0 and 100)\n");
    if (scanf("%i", &factor) == EOF)
        errx(1, "Error, scanf()");

    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image= GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = gtk_image_get_pixbuf(image);

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
    gtk_image_set_from_pixbuf(image, imgPixbuf);
}

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

void callback_FC(GtkMenuItem *menuitem, gpointer user_data)
{
    //variables definitions
    SGlobalData *data = (SGlobalData*)user_data;
    GtkWidget *FCWindow = NULL;
    GtkImage *test_image = NULL;
    GError *err = NULL;
    struct _GdkPixbuf *imgPixbuf = NULL;
    imgPixbuf = gdk_pixbuf_new_from_file(
        "/home/samdiaby/GMPF-by-void-/GMPF_project/image_test.jpg", &err);

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

    //show the filter creator window
    gtk_widget_show(FCWindow);
    menuitem = 0;
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
void callback_setCursor(GtkMenuItem *menuitem, gpointer user_data)
{
     menuitem = 0;
     //init variables
     SGlobalData *data = (SGlobalData*) user_data;
     GdkDisplay *display = NULL;
     GdkCursor *cursor = NULL;
     GdkScreen *screen = NULL;
     GdkWindow * win = NULL;

     //set variables
     screen = gtk_window_get_screen(GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")));
     display = gdk_screen_get_display(screen);

     //create the new cursor
     cursor = gdk_cursor_new_for_display (display, GDK_X_CURSOR);

     //gdk_display_beep (display); play a sound ("beep")

     //set the new cursor on the screen
     win = gdk_screen_get_root_window(screen);
     gdk_window_set_cursor (win, cursor);

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
void callback_create_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr_create(flowbox);
}

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
