#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gdk-pixbuf/gdk-pixbuf.h>
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
void Layer_Free(GMPF_Layer *layer); //TODO: complete




#endif /* LAYER_MANAGER_H */