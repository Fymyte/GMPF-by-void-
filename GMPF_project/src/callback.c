#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "Struct.h"
#include "Matrix.h"
#include "cursor.h"

#include "GMPF_LayerMngr.h"

struct _GdkPixbuf *unchangedPixbuf;


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


void callback_adjust_scale(GtkEntry *entry, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkImage *image = NULL;

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    struct _GdkPixbuf *imgPixbuf = NULL;
    GError *err = NULL;
    if (!unchangedPixbuf)
    {
        imgPixbuf = gdk_pixbuf_new_from_file("gimp_logo.png", &err);
        if(err)
        {
            printf("Error : %s\n", err->message);
            g_error_free(err);
        }
    }
    else
        imgPixbuf = unchangedPixbuf;
    //gtk_image_clear(image);
    const gchar *s = gtk_entry_get_text (entry);
    float scaleValue = atof(s);

    float scaleValue2 = scaleValue / 100;

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);

    int imgwidth = width * scaleValue2;
    int imgheight = height * scaleValue2;

    struct _GdkPixbuf *img2;

        img2 = gdk_pixbuf_scale_simple(imgPixbuf, imgwidth, imgheight,
                scaleValue > 100 ? GDK_INTERP_NEAREST : GDK_INTERP_HYPER);

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

    gtk_image_set_from_pixbuf(image, unchangedPixbuf);
}

void callback_binarize(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Binarize\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

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
    g_object_unref(G_OBJECT(imgPixbuf));

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
