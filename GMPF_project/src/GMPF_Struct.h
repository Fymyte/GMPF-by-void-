#ifndef GMPF_PIXEL_H
#define GMPF_PIXEL_H


#include <gtk/gtk.h>
#include <stdlib.h>

#include "list.h"
#include "types_redefine.h"



// STRUCTURES
typedef enum   GMPF_Tool GMPF_Tool;
typedef struct GMPF_SavedState GMPF_SavedState;
typedef struct GMPF_Pos GMPF_Pos;
typedef struct GMPF_Size GMPF_Size;
typedef struct GMPF_Scale GMPF_Scale;
typedef struct GMPF_Pixel GMPF_Pixel;
typedef struct GMPF_Layer GMPF_Layer;
typedef struct GMPF_LayerMngr GMPF_LayerMngr;

enum GMPF_Tool {
    INCORECT_TOOL = 0,
    PAINTER = 1,
    ERAISER = 2,
    COLOR_PICKER = 3,
    SELECTOR = 4,
    SELECTOR_FREE = 5,
    COLOR_KILLER = 6,
};

struct GMPF_Pos {
    int x, y;
};

struct GMPF_Size {
    int h, w;
};

struct GMPF_Scale {
    float x, y;
};

// TODO: complete the struct GMPF_Pixel
struct GMPF_Pixel {
    unsigned long R, G, B, A;
    //Uint8 R, G, B, A;
};

struct GMPF_SavedState {
    int state;
};

// TODO: complete the struct GMPF_Layer
struct GMPF_Layer {

    char *name; // USE A char[50] TAB FOR A SIMPLE SAVE
    char *filename; // null if it is not based on an image
    // used variable
    GMPF_Pos pos; // Offset on the rendering
    GMPF_Scale scale_factor;
    GMPF_Size size; // Size of the condained surface
    int rotate_angle;
    char isvisible; // Set to one if it has to be rendered

    GdkPixbuf *icon; // The icon displayed in the list of layer
    GdkPixbuf *image; // The pixbuf associated with the layer->surface

    cairo_t *cr; // The cairo context of the surface attribut

    cairo_surface_t *surface;

    // for the list
    GMPF_List list; // The position of the layer in the list

    // to know the UIElement that contains the layer
    GtkImage *UIIcon;
    GtkFlowBoxChild *UIElement;
};


struct GMPF_LayerMngr {
    // basic variable
    GMPF_Size size;
    GMPF_Pos pos;
    int nb_layer;
    GMPF_Tool tool;
    GMPF_List layer_list;

    char *filename;

    cairo_surface_t *surface;
    float brush_size;

    GdkPixbuf *image; //unchungedPixbuf
    GdkPixbuf *display_image; //displayedPixbuf (with scale)


    // get the element on the GUI
    GtkFlowBox *flowbox;
};



#endif /* GMPF_PIXEL_H */
