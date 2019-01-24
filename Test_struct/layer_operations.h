#ifndef LAYER_OPERATIONS_H
#define LAYER_OPERATIONS_H


#include <gdk-pixbuf/gdk-pixbuf.h>
#include <math.h>

#include "types_redefine.h"
#include "layer_manager.h"
#include "GMPF_Pixel.h"



// STRUCTURES



// FUNCTIONS

Uint32 GetPixel(GdkPixbuf *surface, unsigned x, unsigned y); //TODO - Need to be deleted
void PutPixel(GdkPixbuf *surface, unsigned x, unsigned y, GMPF_Pixel pixel); //TODO - Need to be deleted

int IsInSurface(GdkPixbuf *img, int x, int y); //TODO

GdkPixbuf* myRotation (GdkPixbuf *img, int angle); //TODO


void GetPixelRGB(GdkPixbuf *img, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b); //TODO
void SetPixelRGB(GdkPixbuf *img, int x, int y, Uint8 r, Uint8 g, Uint8 b); //TODO

void MovePixelBtwSurface(GdkPixbuf *src, int sx, int sy, 
        GdkPixbuf *dst, int dx, int dy); //TODO



#endif /* LAYER_OPERATIONS_H */