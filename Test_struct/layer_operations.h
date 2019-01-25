#ifndef LAYER_OPERATIONS_H
#define LAYER_OPERATIONS_H


#include <gdk-pixbuf/gdk-pixbuf.h>
#include <math.h>

#include "types_redefine.h"
#include "GMPF_Struct.h"



// STRUCTURES



// FUNCTIONS

GMPF_Pixel * LayerGetPixel(GMPF_Layer *surface, unsigned x, unsigned y); //TODO
void LayerPutPixel(GMPF_Layer *surface, unsigned x, unsigned y, GMPF_Pixel *pixel);

int IsInLayer(GMPF_Layer *layer, int x, int y);

GdkPixbuf * LayerRotation (GMPF_Layer *layer, double rad_angle); //TODO: soon finish


void GetPixelRGB(GMPF_Layer *img, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b); //TODO - Need to be deleted
void SetPixelRGB(GMPF_Layer *img, int x, int y, Uint8 r, Uint8 g, Uint8 b); //TODO - Need to be deleted

void MovePixelBtwLayer(GMPF_Layer *src, int sx, int sy, 
        GMPF_Layer *dst, int dx, int dy); //TODO



#endif /* LAYER_OPERATIONS_H */