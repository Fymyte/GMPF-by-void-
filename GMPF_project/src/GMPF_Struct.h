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
typedef struct GMPF_Selection GMPF_Selection;
typedef struct GMPF_LayerMngr GMPF_LayerMngr;

/*
 * Enumeration of the different tools for the application
 * (Incorect, Painter,Eraiser, Color Picker, Color Killer, Selector and
 * Selector Free properties)
 */
enum GMPF_Tool {
    INCORECT_TOOL = 0,
    PAINTER = 1,
    ERAISER = 2,
    COLOR_PICKER = 3,
    COLOR_KILLER = 4,
    SELECTOR = 5,
    SELECTOR_FREE = 6,
};


/*
 * Structure to store the state of the project
 * (saved propertie)
 */
struct GMPF_SavedState {
    int state;
};


/*
 * As structure to store a position
 * (x and y position properties)
 */
struct GMPF_Pos {
    int x, y;
};

/*
 * A structure to store the selection
 * of the user
 */
struct GMPF_Selection {
    cairo_surface_t *surface;
    GMPF_Pos pos;
};

/*
 * A structure to store ar size
 * (width and height properties)
 */
struct GMPF_Size {
    int h, w;
};


/*
 * A structure to store a scale factor
 * (x and y scale properties)
 */
struct GMPF_Scale {
    float x, y;
};


/*
 * A structure to store a pixel data
 * (red, green, blue and alpha properties)
 */
struct GMPF_Pixel {
    unsigned long R, G, B, A;
    //Uint8 R, G, B, A;
};


/*
 * A structure to store a Layer data
 * A name (char *): the name of the Layer displayed in the Layer list
 * A filename (char *): where the associated image is stored, or NULL if there is no one
 * A scale factor (GMPF_Scale): to know the scale factor to apply when display it on the surface
 * A size (GMPF_Size): to know the excact size of the Layer, indepemdently from the image's size
 * A rotate angle (int): to know from how many degrees, the layer has been rotated
 * A is visible (int): the boolean information to now if we have to diplay this Layer on the surface
 * An icon (GdkPixbuf *): to show the surface in the top right corner, in the list of Layer
 * An image (GdkPixbuf *): the pixbuf associated with the surface to display
 * A cr (cairo_t *): to not create a new one eatch time
 * A surface (cairo_surface_t *): to store all pixels data and diplay in on the Main surface
 * A list (GMPF_List): to know it's position in the Layer list
 * A UIElement (GtkFlowBoxChild *): to know the UI element this Layer is associated with
 */
struct GMPF_Layer {

    char *name;
    char *filename;

    GMPF_Pos pos;
    GMPF_Scale scale_factor;
    GMPF_Size size;
    int rotate_angle;
    char isvisible; // Set to one if it has to be rendered

    GdkPixbuf *icon; // The icon displayed in the list of layer
    GdkPixbuf *image; // The pixbuf associated with the layer->surface

    cairo_t *cr; // The cairo context of the surface attribut

    cairo_surface_t *surface;

    GMPF_List list;

    GtkImage *UIIcon;
    GtkFlowBoxChild *UIElement;
};


/*
 * A structure to store a LayerMngr data
 * A size (GMPF_Size): to store the current size of the project
 * A pos (GMPF_Pos): to store the last position of the cursor before move
 * A nb_layer (int): to store the number of Layer in the Layer list
 * A layer_list (GMPF_List): to store the list of Layer
 * A Selection : to store the user's selection
 * A tool (GMPF_Tool): To store the current selected tool
 * A filename (char *): to store the name of the project's file
 * A brush_size (int): to store the current size of the brush
 * A flowbox (GtkFlowBox): to store the UI element the LayerMngr is associated with+
 */

struct GMPF_LayerMngr {

    GMPF_Size size;
    GMPF_Pos pos;
    int nb_layer;
    GMPF_Tool tool;
    GMPF_List layer_list;

    char *filename;

    cairo_surface_t *surface;

    float brush_size;

    GdkPixbuf *image;
    GdkPixbuf *display_image;

    GtkFlowBox *flowbox;
};


/*
 * A matrix strcuture to store data inside of an array in a conveignant way
 * (rows, cols and mat properties)
 */
struct Matrix
{
    int rows;
    int cols;
    double *mat;
};


/*
 * A structure to store and image composed of pixels inside of matrix structures
 * (rows, cols, red, gree, blue and alpha properties)
 */
struct Img_rgb
{
    int rows;
    int cols;

    struct Matrix *red;
    struct Matrix *green;
    struct Matrix *blue;
    struct Matrix *alpha;
};

#endif /* GMPF_PIXEL_H */
