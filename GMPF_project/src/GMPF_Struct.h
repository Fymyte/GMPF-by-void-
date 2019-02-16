#ifndef GMPF_PIXEL_H
#define GMPF_PIXEL_H


#include <gtk/gtk.h>
#include <stdlib.h>

#include "types_redefine.h"



// STRUCTURES

typedef struct GMPF_Pos GMPF_Pos;
typedef struct GMPF_Size GMPF_Size;
typedef struct GMPF_Pixel GMPF_Pixel;
typedef struct GMPF_Layer GMPF_Layer;
typedef struct GMPF_LayerMngr GMPF_LayerMngr;

struct GMPF_Pos {
    int x, y;
};

struct GMPF_Size {
    int h, w;
};

// TODO: complete the struct GMPF_Pixel
struct GMPF_Pixel {
    Uint8 R, G, B, A;
};

// TODO: complete the struct GMPF_Layer
struct GMPF_Layer {
    // used variable
    GMPF_Size img_size;
    GdkPixbuf *image;

    // for the list
    GMPF_Layer *next;

    // to know the UIElement that contains the layer
    GtkFlowBoxChild *UIElement;
};

struct GMPF_LayerMngr {
    // basic variable
    int nb_layer; // unused I think
    GMPF_Layer *layer_list;

    // get the element on the GUI
    GtkFlowBox *UIElement;
};






// FUNCTIONS

// use std init and free for GMPF_Pos, GMPF_Size & GMPF_Pixel

//GMPF_Layer * free_GMPF_Layer(GMPF_Layer *layer);

GMPF_LayerMngr * init_GMPF_LayerMngr(GtkFlowBox *UIElement);
void free_GMPF_LayerMngr(GMPF_LayerMngr *layermngr);



#endif /* GMPF_PIXEL_H */
