#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gtk/gtk.h>
#include <stdlib.h>
#include <err.h> // for the errors
#include <stdio.h> // for the errors

#include "types_redefine.h"
#include "GMPF_Struct.h"


// DEFINES
#define LAYERMNGR_KEY_NAME "GMPF_layermngr"
#define LAYER_KEY_NAME "GMPF_layer"


// STRUCTURES







// FUNCTIONS

GMPF_Layer * Layer_CreateFromFile(const char *filename); //TODO: complete

//
// for the GtkFlowBox - interact with Gtk
//
void layermngr_initialization(GtkFlowBox *flowbox);
void layermngr_clear(GtkFlowBox *flowbox);
void layermngr_delete(GtkFlowBox *flowbox);

//
// for the GMPF_LayerMngr
//
GMPF_Layer * layermngr_get_selected_layer(GtkFlowBox *flowbox);
            // maybe change this function
void layermngr_add_new_layer(GtkFlowBox *flowbox);
void layermngr_delete_selected_layer(GtkFlowBox *flowbox);


//
// for the GMPF_Layer
//
GMPF_Layer * layer_initialization();
void layer_delete(GMPF_Layer *layer);



#endif /* LAYER_MANAGER_H */