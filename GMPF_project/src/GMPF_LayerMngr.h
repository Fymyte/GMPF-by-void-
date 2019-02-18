#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gtk/gtk.h>
#include <stdlib.h>
#include <err.h> // for the errors
#include <stdio.h> // for the errors

#include "types_redefine.h"
#include "GMPF_Struct.h"
#include "list.h"


// DEFINES
#define LAYERMNGR_KEY_NAME "GMPF_layermngr"
#define LAYER_KEY_NAME "GMPF_layer"


// STRUCTURES







// FUNCTIONS

GMPF_Layer * Layer_CreateFromFile(const char *filename); //TODO: complete
    // don't use this function

//
// for the GtkFlowBox - interact with Gtk
//
void layermngr_create(GtkFlowBox *flowbox); //ENDED
void layermngr_initialization(GMPF_LayerMngr *layermngr); //ENDED
void layermngr_clear(GtkFlowBox *flowbox); //ENDED
void layermngr_delete(GtkFlowBox *flowbox); //ENDED

//
// for the GMPF_LayerMngr
//
GMPF_Layer * layermngr_get_selected_layer(GtkFlowBox *flowbox); //ENDED
void layermngr_add_new_layer(GtkFlowBox *flowbox); //ENDED
void layermngr_delete_selected_layer(GtkFlowBox *flowbox); //ENDED


//
// for the GMPF_Layer
//
GMPF_Layer * layer_initialization(); //ENDED
void layer_delete(GMPF_Layer *layer); //ENDED


//
// for Operations on GMPF_Layer
//
int layer_get_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel);
int layer_put_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel);

void layer_rotation(GtkFlowBox *flowbox, double rad_angle);
void layer_rotation_right(GtkFlowBox *flowbox);
void layer_rotation_left(GtkFlowBox *flowbox);






#endif /* LAYER_MANAGER_H */