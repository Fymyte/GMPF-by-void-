#include <gtk-3.0/gtk/gtk.h>
#include <err.h>

#include "gtk.h"

int
main(int argc, char *argv [])
{
    GtkWidget *Main_window = NULL;

    /* Initialisation de la librairie Gtk. */
    gtk_init(&argc, &argv);

    /* Récupération du pointeur de la fenêtre principale */
    Main_window = GMPF_start();

    if (Main_window == NULL)
    {
        errx(1, "Unable to get main Window");
    }

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all (Main_window);

    gtk_main();

    return 0;
}
