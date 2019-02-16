#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gtk/gtk.h>
#include <stdlib.h>
#include <err.h> // for the errors
#include <stdio.h> // for the errors

#include "types_redefine.h"
#include "GMPF_Struct.h"


// STRUCTURES







// FUNCTIONS

GMPF_Layer * Layer_CreateFromFile(const char *filename); //TODO: complete


GMPF_Layer * LayerMngr_get_selected_layer(GMPF_LayerMngr *layermngr);


void LayerMngr_Append_Layer(GMPF_LayerMngr *layermngr);
// GMPF_Layer * LayerMngr_Get_Layer(GMPF_LayerMngr *layermngr, int layernum);
void LayerMngr_Delete_Layer(GMPF_LayerMngr *layermngr, int layernum);



#endif /* LAYER_MANAGER_H */