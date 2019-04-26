#ifndef CALLBACK_H_
#define CALLBACK_H_

/*************************Includes from Standard C library*********************/
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <cairo.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <gtk-3.0/gtk/gtk.h>

/**************************Includes from Personal Files************************/
#include "cursor.h"
#include "GMPF_LayerMngr.h"
#include "Matrix.h"
#include "Img_rgb.h"
#include "pixel_op.h"
#include "brush.h"
#include "saving.h"
#include "macro.h"
#include "filters.h"
#include "fcreator.h"
#include "Channels.h"


int check(int width, int height, int i, int j);
//cairo image functions

/***********Functions to open, load and save project and images****************/
int open_confirm_quit_without_saving_dialog(gpointer user_data);
gboolean GMPF_save_project(gpointer user_data);
gboolean GMPF_save_under_project(gpointer user_data);
void load_image_cairo(GtkWindow *window,
                      GMPF_LayerMngr *layermngr,
                      GtkFlowBox *flowbox,
                      char *filename);

void reset_cursor(GtkMenuItem *menuitem,
                  gpointer     user_data);

//drawing functions
void draw_brush (GtkWidget  *widget,
                 gdouble    x,
                 gdouble    y,
                 gpointer   user_data);
void draw_rubber (GtkWidget *widget,
                  gdouble     x,
                  gdouble     y,
                  gpointer    user_data);


#endif
