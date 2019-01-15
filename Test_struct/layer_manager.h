#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

#include "types_redefine.h"

//
//
// Maybe split the file by struct
//
//

// STRUCTURES


// TODO: complete the struct GMPF_Layer_Manager
struct GMPF_Layer_Manager {
    int nb_layer;
} GMPF_Layer_Manager ;


// TODO: complete the struct GMPF_Layer
struct GMPF_Layer {
    GdkPixbuf *img; // or without pointer
    GMPF_Image image;
} GMPF_Layer;


// TODO: complete the struct GMPF_Image
struct GMPF_Image {
    unsigned int height;
    unsigned int width;
    GMPF_Pixel *image; // or Pixbuf
} GMPF_Image;


// TODO: complete the struct GMPF_Pixel
struct GMPF_Pixel {
    Uint8 A, R, G, B;
} GMPF_Pixel;



// FUNCTIONS



#endif /* LAYER_MANAGER_H */