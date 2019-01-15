#ifndef LAYER_OPERATIONS_H
#define LAYER_OPERATIONS_H

#include "types_redefine.h"



// STRUCTURES



// FUNCTIONS

Uint32 getpixel(GdkPixbuf *surface, unsigned x, unsigned y); //TODO - Need to be deleted
void putpixel(GdkPixbuf *surface, unsigned x, unsigned y, Uint32 pixel); //TODO - Need to be deleted

int IsInSurface(GdkPixbuf *img, int x, int y); //TODO

GdkPixbuf* myRotation (GdkPixbuf *img, double rad_angle); //TODO


void GetPixelRGB(GdkPixbuf *img, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b); //TODO
void SetPixelRGB(GdkPixbuf *img, int x, int y, Uint8 r, Uint8 g, Uint8 b); //TODO

void MovePixelBtwSurface(GdkPixbuf *src, int sx, int sy, 
        GdkPixbuf *dst, int dx, int dy); //TODO



#endif /* LAYER_OPERATIONS_H */