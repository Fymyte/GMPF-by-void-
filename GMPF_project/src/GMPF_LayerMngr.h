#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gtk/gtk.h>
#include <stdlib.h>
#include <err.h> // for the errors
#include <stdio.h> // for the errors

#include "types_redefine.h"
#include "GMPF_Struct.h"

//
//
// Maybe split the file by struct
//
//

// STRUCTURES







// FUNCTIONS

GMPF_Layer * Layer_CreateFromFile(const char *filename); //TODO: complete
//void Layer_Free(GMPF_Layer *layer); //TODO: complete


void LayerMngr_Append_Layer(GMPF_LayerMngr *layermngr);
GMPF_Layer * LayerMngr_Get_Layer(GMPF_LayerMngr *layermngr, int layernum);
void LayerMngr_Remove_Layer(GMPF_LayerMngr *layermngr, int layernum);



#endif /* LAYER_MANAGER_H */