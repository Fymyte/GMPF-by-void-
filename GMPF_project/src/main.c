#include <gtk-3.0/gtk/gtk.h>
#include <err.h>

#include "gtk.h"

int main(int argc, char *argv [])
{
    int error;
    glob_init();
    /* Initialisation de la librairie Gtk. */
    gtk_init(&argc, &argv);

    /* Récupération du pointeur de la fenêtre principale */
    error = GMPF_start();

    return error;
}
