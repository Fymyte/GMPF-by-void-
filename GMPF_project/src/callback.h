#ifndef CALLBACK_H_
#define CALLBACK_H_

//#define DEBUG 1 // Use for only print debuging info if true
//#define D_PRINT(fmt, ...) \
//        do { if (DEBUG) fprintf(stderr, "debug: %s:%d:%s(): " fmt, __FILE__, \
//                                __LINE__, __func__, __VA_ARGS__); } while (0)

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <cairo.h>
#include <gtk-3.0/gtk/gtk.h>


#include "cursor.h"
#include "GMPF_LayerMngr.h"
#include "Struct.h"
#include "Matrix.h"
#include "Img_rgb.h"
#include "pixel_op.h"
#include "brush.h"
#include "Save.h"
#include "debug.h"

//callback functions

//cairo image functions
void draw_line(cairo_t *cr);
void on_draw_event(GtkWidget * widget, cairo_t *cr, gpointer user_data);

int check(int width, int height, int i, int j);

void adjust_scale(double scale_x, double scale_y, gpointer user_data);
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
void callback_FC(GtkMenuItem *menuitem, gpointer user_data);
void callback_vertical(GtkMenuItem *menuitem, gpointer user_data);
void callback_tinter(GtkMenuItem *menuitem, gpointer user_data);
void callback_colorfull(GtkMenuItem *menuitem, gpointer user_data);
void reset_cursor(GtkMenuItem *menuitem, gpointer user_data);
void callback_horizontal(GtkMenuItem *menuitem, gpointer user_data);
void callback_negative(GtkMenuItem *menuitem, gpointer user_data);

//tools functions
void callback_brush(GtkMenuItem *menuitem, gpointer user_data);
void callback_rubber(GtkMenuItem *menuitem, gpointer user_data);
void callback_resize_brush(GtkEntry *entry, gpointer user_data);

//drawing functions
void clear_surface (gpointer user_data);
gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer user_data);
gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer user_data);
void draw_brush (GtkWidget *widget, gdouble x, gdouble y, gpointer user_data);
gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
gboolean motion_notify_event_cb (GtkWidget *widget, GdkEventMotion *event, gpointer user_data);
void draw_rubber (GtkWidget *widget, gdouble x, gdouble y,gpointer user_data);

//
//     // GMPF_LayerMngr
// void callback_clear_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);
// void callback_add_GMPF_Layer(GtkMenuItem *menuitem, gpointer user_data);
// void callback_remove_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);

#endif
