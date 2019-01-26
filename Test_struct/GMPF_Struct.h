#ifndef GMPF_PIXEL_H
#define GMPF_PIXEL_H


#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdlib.h>

#include "types_redefine.h"



// STRUCTURES

// TODO: complete the struct GMPF_Layer_Manager
struct GMPF_Layer_Manager {
    int nb_layer;
    GMPF_Layer *layer_list;
} GMPF_Layer_Manager ;


// TODO: complete the struct GMPF_Layer
struct GMPF_Layer {
    GMPF_Size img_size;
    GdkPixbuf *image;
    GMPF_Layer *next;
} GMPF_Layer;


// TODO: complete the struct GMPF_Pixel
struct GMPF_Pixel {
    Uint8 R, G, B, A;
} GMPF_Pixel;


// TODO: complete the struct GMPF_Size
struct GMPF_Size {
    int h, w;
} GMPF_Size;


// TODO: complete the struct GMPF_Size
struct GMPF_Pos {
    int x, y;
} GMPF_Pos;


// FUNCTIONS

GMPF_Layer_Manager *init_GMPF_Layer_Manager(); //TODO
void free_GMPF_Layer_Manager(GMPF_Layer_Manager *layer_mng); //TODO

GMPF_Layer *init_GMPF_Layer(GMPF_Layer_Manager *layer_mng); //TODO
void free_GMPF_Layer(GMPF_Layer *layer, GMPF_Layer_Manager *layer_mng); //TODO

GMPF_Pixel *init_GMPF_Pixel(); //TODO
void free_GMPF_Pixel(GMPF_Pixel *pixel); //TODO

GMPF_Size *init_GMPF_Size(); //TODO
void free_GMPF_Size(GMPF_Size *size); //TODO

GMPF_Pos *init_GMPF_Pos(); //TODO
void free_GMPF_Pos(GMPF_Pos *pos); //TODO


#endif /* GMPF_PIXEL_H */
