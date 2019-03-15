#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "cursor.h"

#include "GMPF_LayerMngr.h"

/*typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;
*/

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

    GtkWidget *da = NULL;
    da = GTK_WIDGET(gtk_builder_get_object(data.builder, "drawingArea"));
    gtk_widget_set_events (da, gtk_widget_get_events (da)
                                | GDK_BUTTON_PRESS_MASK
                                | GDK_POINTER_MOTION_MASK);



    gtk_widget_show_all (Main_window);

    gtk_main();

    return 0;
}


void GMPFquit(GtkMenuItem *menuitem, gpointer user_data)
{
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    resetCursor(data);
    gtk_main_quit();
}


    // GMPF_LayerMngr
void callback_clear_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data)
{
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr_clear(flowbox);
}

void callback_add_GMPF_Layer(GtkMenuItem *menuitem, gpointer user_data)
{
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr_add_new_layer(flowbox, NULL);
}

void callback_remove_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data)
{
    menuitem = 0;
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFlowBox *flowbox = NULL;
    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr_delete_selected_layer(flowbox);
}
