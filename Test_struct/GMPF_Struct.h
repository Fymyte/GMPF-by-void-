#ifndef GMPF_PIXEL_H
#define GMPF_PIXEL_H


#include <gdk-pixbuf/gdk-pixbuf.h>

#include "types_redefine.h"



// STRUCTURES

// TODO: complete the struct GMPF_Layer_Manager
struct GMPF_Layer_Manager {
    int nb_layer;
} GMPF_Layer_Manager ;


// TODO: complete the struct GMPF_Layer
struct GMPF_Layer {
    GMPF_Size img_size;
    GdkPixbuf *image;
} GMPF_Layer;


// TODO: complete the struct GMPF_Pixel
struct GMPF_Pixel {
    Uint8 R, G, B, A;
} GMPF_Pixel;


// TODO: complete the struct GMPF_Size
struct GMPF_Size {
    int h, w;
} GMPF_Size;


// FUNCTIONS



#endif /* GMPF_PIXEL_H */