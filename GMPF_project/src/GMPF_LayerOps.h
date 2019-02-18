#ifndef LAYER_OPERATIONS_H
#define LAYER_OPERATIONS_H


#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

#include "types_redefine.h"
#include "GMPF_Struct.h"

/*
    Don't use this file.
*/

// STRUCTURES



// FUNCTIONS

void LayerGetPixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel); // FINISHED / need to test
void LayerPutPixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel); // FINISHED / need to test

int IsInLayer(GMPF_Layer *layer, GMPF_Pos *pos); // FINISHED / need to test

GdkPixbuf * LayerRotation (GMPF_Layer *layer, double rad_angle); //TODO: soon finish

// need to return an int to say if it works
void MovePixelBtwLayer(GMPF_Layer *src, GMPF_Pos *srcpos, 
        GMPF_Layer *dst, GMPF_Pos *dstpos); //TODO: soon finish



#endif /* LAYER_OPERATIONS_H */