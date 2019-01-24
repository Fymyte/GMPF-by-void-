#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gdk-pixbuf/gdk-pixbuf.h>

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
    GdkPixbuf image;
} GMPF_Image;




// FUNCTIONS



#endif /* LAYER_MANAGER_H */