#ifndef GMPF_PIXEL_H
#define GMPF_PIXEL_H


#include <gtk/gtk.h>
#include <stdlib.h>

#include "list.h"
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
    GMPF_Pos pos;
    GMPF_Size size;
    char isvisible;

    GdkPixbuf *icon;
    GdkPixbuf *image;

    cairo_t *cr;

    cairo_surface_t *unchanged_surface;
    cairo_surface_t *surface;

    // for the list
    GMPF_List list;

    // to know the UIElement that contains the layer
    GtkImage *UIIcon;
    GtkFlowBoxChild *UIElement;
};


struct GMPF_LayerMngr {
    // basic variable
    GMPF_Size size;
    int nb_layer;
    GMPF_List layer_list;

    cairo_surface_t *surface;

    GdkPixbuf *image; //unchungedPixbuf
    GdkPixbuf *display_image; //displayedPixbuf (with scale)


    // get the element on the GUI
    GtkFlowBox *flowbox;
};






// FUNCTIONS




#endif /* GMPF_PIXEL_H */
