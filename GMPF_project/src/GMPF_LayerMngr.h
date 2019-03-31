#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gtk/gtk.h>
#include <stdlib.h>
#include <err.h> // for the errors
#include <stdio.h> // for the errors

#include "types_redefine.h"
#include "GMPF_Struct.h"
#include "list.h"

#include "macro.h"


// DEFINES
#define LAYERMNGR_KEY_NAME "GMPF_layermngr"
#define LAYER_KEY_NAME "GMPF_layer"

#define INIT_LAYER_UI(_layerui) \
    gtk_widget_set_sensitive(image, TRUE); \
    gtk_widget_set_visible(image, TRUE);\
    /*** Visual ***/ \
    /* size */ \
    gtk_widget_set_size_request(image, 160, 90); \
    /* Alignement */ \
    gtk_widget_set_halign(image, GTK_ALIGN_START); \
    gtk_widget_set_valign(image, GTK_ALIGN_START); \
    /* Margin */ \
    gtk_widget_set_margin_top(image, 5); \
    gtk_widget_set_margin_bottom(image, 5); \
    gtk_widget_set_margin_start(image, 5); \
    gtk_widget_set_margin_end(image, 5); \

// STRUCTURES







// FUNCTIONS


//
// for the GtkFlowBox - interact with Gtk
//
void layermngr_create(GtkFlowBox *flowbox); //ENDED
void layermngr_set_to_flowbox(GtkFlowBox *flowbox, GMPF_LayerMngr *layermngr);
void layermngr_initialization(GMPF_LayerMngr *layermngr); //ENDED
void layermngr_clear(GtkFlowBox *flowbox); //ENDED
void layermngr_delete(GtkFlowBox *flowbox); //ENDED

void layermngr_move_down_selected_layer(GtkFlowBox *flowbox);
void layermngr_move_up_selected_layer(GtkFlowBox *flowbox);


//
// for the GMPF_LayerMngr
//
GMPF_Layer * layermngr_get_selected_layer(GtkFlowBox *flowbox); //ENDED
GMPF_Layer *layermngr_add_new_layer(GtkFlowBox *flowbox, const char *filename); //ENDED
void layermngr_delete_selected_layer(GtkFlowBox *flowbox); //ENDED

GMPF_LayerMngr *layermngr_get_layermngr(GtkFlowBox *flowbox);

/*void layermngr_display_refresh(GtkFlowBox *flowbox);
*/

//
// for the GMPF_Layer
//
GMPF_Layer * layer_initialization(); //ENDED
void layer_delete(GMPF_Layer *layer); //ENDED
void layer_icon_refresh(GMPF_Layer *layer);


//
// for Operations on GMPF_Layer
//
int layer_get_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel);
int layer_put_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel);

/*void layer_rotation(GtkFlowBox *flowbox, double rad_angle);
void layer_rotation_right(GtkFlowBox *flowbox);
void layer_rotation_left(GtkFlowBox *flowbox);*/


//
// for GdkPixbuf standardization
//
GdkPixbuf * new_pixbuf_standardized(GMPF_Size *size);
int pixbuf_standardized(GdkPixbuf **pixbuf);





#endif /* LAYER_MANAGER_H */
