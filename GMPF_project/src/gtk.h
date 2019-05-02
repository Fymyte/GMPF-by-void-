#ifndef GTK_H
#define GTK_H

#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "callback.h"

#define INTERFACE_NAME "interface.glade"

#define CSSTHEME "themes/main.css"

int GMPF_start();
void GMPF_quit (GtkFlowBox *flowbox,
                GtkWindow  * window);

#endif
