#ifndef CALLBACK_H_
#define CALLBACK_H_

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



//callback functions
void draw_line(cairo_t *cr);
void on_draw_event(GtkWidget * widget, cairo_t *cr, gpointer user_data);

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
//void callback_setCursor(gpointer user_data);
void callback_FC(GtkMenuItem *menuitem, gpointer user_data);
void callback_vertical(GtkMenuItem *menuitem, gpointer user_data);
void callback_tinter(GtkMenuItem *menuitem, gpointer user_data);
void callback_colorfull(GtkMenuItem *menuitem, gpointer user_data);
void reset_cursor(GtkMenuItem *menuitem, gpointer user_data);
void callback_horizontal(GtkMenuItem *menuitem, gpointer user_data);
void callback_negative(GtkMenuItem *menuitem, gpointer user_data);
void callback_brush(GtkMenuItem *menuitem, gpointer user_data);
void callback_rubber(GtkMenuItem *menuitem, gpointer user_data);

//drawing functions
void clear_surface (void);
gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data);
gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data);
void draw_brush (GtkWidget *widget, gdouble x, gdouble y);
gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean motion_notify_event_cb (GtkWidget *widget, GdkEventMotion *event, gpointer data);
void draw_rubber (GtkWidget *widget, gdouble x, gdouble y);

//
//     // GMPF_LayerMngr
// void callback_clear_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);
// void callback_add_GMPF_Layer(GtkMenuItem *menuitem, gpointer user_data);
// void callback_remove_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);

#endif
