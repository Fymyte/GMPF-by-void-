#include "callback.h"

struct _GdkPixbuf *unchangedPixbuf;
struct {
  cairo_surface_t *image;
} glob;


static void do_drawing(cairo_t *cr);
static gboolean on_draw_event(GtkWidget * widget, cairo_t *cr, gpointer user_data);

int check(int width, int height, int i, int j);
void callback_flip(GtkMenuItem *menuitem, gpointer user_data);
void callback_rotate(GtkMenuItem *menuitem, gpointer user_data);
void callback_hideWidget(GtkWidget *widget, gpointer user_data);
void callback_about (GtkMenuItem *menuitem, gpointer user_data);
void callback_adjust_scale(GtkEntry *entry, gpointer user_data);
void callback_image(GtkFileChooser *filebtn, gpointer user_data);
void callback_image(GtkFileChooser *filebtn, gpointer user_data);
void callback_image_cairo(GtkFileChooser *btn, gpointer user_data);
void callback_binarize(GtkMenuItem *menuitem, gpointer user_data);
void callback_binarize_color(GtkMenuItem *menuitem, gpointer user_data);
void callback_convolute_f(GtkMenuItem *menuitem, gpointer user_data);
void callback_grey(GtkMenuItem *menuitem, gpointer user_data);
void callback_setCursor(GtkMenuItem *menuitem, gpointer user_data);
void callback_FC(GtkMenuItem *menuitem, gpointer user_data);
void callback_vertical(GtkMenuItem *menuitem, gpointer user_data);
void callback_tinter(GtkMenuItem *menuitem, gpointer user_data);
void callback_colorfull(GtkMenuItem *menuitem, gpointer user_data);
void reset_cursor(GtkMenuItem *menuitem, gpointer user_data);
void callback_horizontal(GtkMenuItem *menuitem, gpointer user_data);
void callback_negative(GtkMenuItem *menuitem, gpointer user_data);


int check(int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i > width || j > height)
        return 0;
    return 1;
}

void callback_flip(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkImage *image = NULL;

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));
    GdkPixbuf *pixbuf;

    const char *menulabel = gtk_menu_item_get_label (menuitem);
    g_print("%s\n", menulabel);

    if (strcmp(menulabel, "Flip horizontal"))
        pixbuf = gdk_pixbuf_flip (unchangedPixbuf, TRUE);

    else
        pixbuf = gdk_pixbuf_flip (unchangedPixbuf, FALSE);

    g_object_unref(unchangedPixbuf);
    unchangedPixbuf = pixbuf;
    gtk_image_set_from_pixbuf(image, pixbuf);
}


void callback_rotate(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkImage *image = NULL;

    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));
    GdkPixbuf *pixbuf;

    const char *menulabel = gtk_menu_item_get_label (menuitem);
    g_print("%s\n", menulabel);
    if (strcmp(menulabel, "Rotate left"))
    {
        pixbuf = gdk_pixbuf_rotate_simple(unchangedPixbuf, GDK_PIXBUF_ROTATE_CLOCKWISE);
    }
    else
    {
        pixbuf = gdk_pixbuf_rotate_simple(unchangedPixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
    }

    g_object_unref(unchangedPixbuf);
    unchangedPixbuf = pixbuf;
    gtk_image_set_from_pixbuf(image, pixbuf);
}


void callback_hideWidget(GtkWidget *widget, gpointer user_data)
{
    gtk_widget_hide(widget);
    user_data = 0;
}


void callback_about (GtkMenuItem *menuitem, gpointer user_data)
{
    /* Transtypage du pointeur user_data pour récupérer nos données. */
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *dialog = NULL;

    dialog =  gtk_about_dialog_new ();

    /* Pour l'exemple on va rendre la fenêtre "À propos" modale par rapport à la */
    /* fenêtre principale. */
    gtk_window_set_transient_for (GTK_WINDOW(dialog),
            GTK_WINDOW(gtk_builder_get_object (data->builder, "MainWindow")));

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    menuitem = 0;
}


void callback_adjust_scale(GtkEntry *entry, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    GtkWidget *da = NULL;
    GtkWidget *layout = NULL;
    GtkFlowBox *flowbox = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
    layout = GTK_WIDGET(gtk_builder_get_object(data->builder, "Layout"));
    layermngr = layermngr_get_layermngr(flowbox);

    struct _GdkPixbuf *imgPixbuf = NULL;
    GError *err = NULL;
    if (!(layermngr->image))
    {
        imgPixbuf = gdk_pixbuf_new_from_file("GMPF.png", &err);
        if(err)
        {
            printf("Error : %s\n", err->message);
            g_error_free(err);
        }
    }
    else
        imgPixbuf = layermngr->image;

    const gchar *s = gtk_entry_get_text (entry);
    float scaleValue = atof(s);

    float scaleValue2 = scaleValue / 100;
    if (scaleValue2 <= 0)
    {
        scaleValue2 = 1;
    }
    g_message("scaleValue = %f", scaleValue);

    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);

    int imgwidth = width * scaleValue2;
    int imgheight = height * scaleValue2;
    g_message("base: %i*%i, after:%i*%i", width, height, imgwidth, imgheight);

    // Set the new size of the drawingArea
    gtk_widget_set_size_request(da, imgwidth, imgheight);
    // Set the new size of the layout that contain the drawingArea
    gtk_layout_set_size((GtkLayout *)layout, imgwidth * 1.1, imgheight * 1.1);
    // Apply the scaling on pixbuf
    layermngr->display_image = gdk_pixbuf_scale_simple(imgPixbuf, imgwidth, imgheight,
                scaleValue > 100 ? GDK_INTERP_NEAREST : GDK_INTERP_HYPER);


    glob.image = gdk_cairo_surface_create_from_pixbuf(layermngr->display_image, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
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
    if (!gdk_pixbuf_get_has_alpha(unchangedPixbuf))
    {
         GdkPixbuf *i = gdk_pixbuf_add_alpha ((const GdkPixbuf *)unchangedPixbuf, FALSE, 0, 0, 0);
        g_object_unref (unchangedPixbuf);
        unchangedPixbuf = i;
    }

    int height = gdk_pixbuf_get_height(unchangedPixbuf);
    int width = gdk_pixbuf_get_width(unchangedPixbuf);

    GtkWidget *MainWindow = GTK_WIDGET(gtk_builder_get_object(data->builder, "MainWindow"));
    char *title = malloc (sizeof(char) * 1000);
    sprintf(title, "GMPF - %s : %d * %d", filename, width, height);
    gtk_window_set_title (GTK_WINDOW(MainWindow), title);
    free (title);

    gtk_image_set_from_pixbuf(image, unchangedPixbuf);
}


static void do_drawing(cairo_t *cr)
{
    cairo_set_source_surface(cr, glob.image, 0, 0);
    cairo_paint(cr);
}


static gboolean on_draw_event(GtkWidget * widget, cairo_t *cr, gpointer user_data)
{
    do_drawing(cr);
    user_data = 0;
    widget = 0;
    return FALSE;
}


void callback_image_cairo(GtkFileChooser *btn, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    GError *error = NULL;
    GMPF_LayerMngr *layermngr = NULL;

    gchar *filename = gtk_file_chooser_get_filename(btn);
    /*
        Ici c'est chez moi
    */
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);
    layermngr_add_new_layer(flowbox, filename);

    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
    if(da == NULL)
        printf("toz\n");

    // layout = GTK_WIDGET(gtk_builder_get_object(data->builder, "Layout"));

    layermngr->image = gdk_pixbuf_new_from_file(filename, &error);
    glob.image = gdk_cairo_surface_create_from_pixbuf(layermngr->image, 0, NULL);

    int width = gdk_pixbuf_get_width(layermngr->image);
    int height = gdk_pixbuf_get_height(layermngr->image);
	gtk_widget_set_size_request(da, width, height);

    if(error)
    {
        printf("Error : %s\n", error->message);
        g_error_free(error);
    }

    if (!gdk_pixbuf_get_has_alpha(layermngr->image))
    {
        GdkPixbuf *i = gdk_pixbuf_add_alpha ((const GdkPixbuf *)layermngr->image, FALSE, 0, 0, 0);
        g_object_unref (layermngr->image);
        layermngr->image = i;
    }

    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);

}


void callback_binarize(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Binarize\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
    glob.image = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
}


void callback_binarize_color(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Binarize color\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
    glob.image = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
}


void callback_convolute_f(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Convolution\n");
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

    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkImage *image = NULL;
    image = GTK_IMAGE(gtk_builder_get_object(data->builder, "OriginalImage"));

    GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
                        error = gdkpixbuf_get_colors_by_coordinates(imgPixbuf, i, j, &red, &green, &blue, &alpha);
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
    unchangedPixbuf = imgPixbuf;
    gtk_image_set_from_pixbuf(image, imgPixbuf);

    free_img_rgb(img);
    free(mat);
}


void callback_grey(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Grey\n");
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *da = NULL;
    da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));

    struct _GdkPixbuf *imgPixbuf;
    imgPixbuf = unchangedPixbuf;

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
    glob.image = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
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
     struct _GdkPixbuf *imgPixbuf;
     struct _GdkPixbuf *img2;

     //set variables
     screen = gtk_window_get_screen(GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")));
     display = gdk_screen_get_display(screen);
     GError *error = NULL;
     imgPixbuf = gdk_pixbuf_new_from_file("penta00.gif", &error);
     int width = gdk_pixbuf_get_width(imgPixbuf);
     int height = gdk_pixbuf_get_height(imgPixbuf);
     img2 = gdk_pixbuf_scale_simple(imgPixbuf, width/6, height/6, GDK_INTERP_HYPER);

     //create the new cursor
     cursor = gdk_cursor_new_from_pixbuf(display, img2, 23, 23);
    if(error)
    {
        printf("Error : %s\n", error->message);
        g_error_free(error);
    }
     //gdk_display_beep (display); play a sound ("beep")

     //set the new cursor on the screen
     win = gdk_screen_get_root_window(screen);
     gdk_window_set_cursor (win, cursor);
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
         "image_test.jpg", &err);

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

     //show the filter creator windowjust
     gtk_widget_show(FCWindow);
     menuitem = 0;
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


 void callback_tinter(GtkMenuItem *menuitem, gpointer user_data)
 {
     g_print("Tinter\n");
     menuitem = 0;

     guchar r, g, b, factor;

     SGlobalData *data = (SGlobalData*) user_data;
     GtkWidget *da = NULL;
     GtkColorChooser *colorChooser = NULL;
     GdkRGBA rgba;
     da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
     colorChooser = (GtkColorChooser *)(gtk_builder_get_object(data->builder, "ColorTinter"));
     gtk_color_chooser_get_rgba (colorChooser, &rgba);
     r = (guchar)(rgba.red * 255);
     g = (guchar)(rgba.green * 255);
     b = (guchar)(rgba.blue * 255);
     factor = (guchar)(rgba.alpha * 100);

     struct _GdkPixbuf *imgPixbuf;
     imgPixbuf = unchangedPixbuf;

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
    glob.image = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
 }


 void callback_colorfull(GtkMenuItem *menuitem, gpointer user_data)
 {
     g_print("Colorfull\n");
     menuitem = 0;
     guchar r, g, b, factor;

     SGlobalData *data = (SGlobalData*) user_data;
     GtkWidget *da = NULL;
     GtkColorChooser *colorChooser = NULL;
     GdkRGBA rgba;
     da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));
     colorChooser = (GtkColorChooser *)(gtk_builder_get_object(data->builder, "ColorTinter"));
     gtk_color_chooser_get_rgba (colorChooser, &rgba);
     r = (guchar)(rgba.red * 255);
     g = (guchar)(rgba.green * 255);
     b = (guchar)(rgba.blue * 255);
     factor = (guchar)(rgba.alpha * 255);

     struct _GdkPixbuf *imgPixbuf;
     imgPixbuf = unchangedPixbuf;

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
    glob.image = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
 }


 void callback_negative(GtkMenuItem *menuitem, gpointer user_data)
 {
     g_print("Negative\n");
     menuitem = 0;
     SGlobalData *data = (SGlobalData*) user_data;
     GtkWidget *da = NULL;
     da = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawingArea"));

     struct _GdkPixbuf *imgPixbuf;
     imgPixbuf = unchangedPixbuf;

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
    glob.image = gdk_cairo_surface_create_from_pixbuf(imgPixbuf, 0, NULL);
    g_signal_connect(G_OBJECT(da), "draw", G_CALLBACK(on_draw_event), NULL);
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

void reset_cursor(GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    resetCursor(data);
    menuitem = 0;
}
