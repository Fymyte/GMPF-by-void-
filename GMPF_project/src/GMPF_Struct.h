#ifndef GMPF_STRUCT_H
#define GMPF_STRUCT_H


#include <gtk/gtk.h>
#include <stdlib.h>

#include "types_redefine.h"

#define BUFFER_SIZE 501


// STRUCTURES
typedef enum   GMPF_Tool GMPF_Tool;
typedef enum   GMPF_Brush GMPF_Brush;
typedef enum   GMPF_Action GMPF_Action;
typedef struct GMPF_Buffer GMPF_Buffer;
typedef struct GMPF_BufferElement GMPF_BufferElement;
typedef struct GMPF_List GMPF_List;
typedef struct savebuf s_savebuf;
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
    GMPF_TOOL_INCORECT = 0,
    GMPF_TOOL_PAINTER = 1,
    GMPF_TOOL_ERAISER = 2,
    GMPF_TOOL_COLOR_PICKER = 3,
    GMPF_TOOL_COLOR_KILLER = 4,
    GMPF_TOOL_SELECTOR = 5,
    GMPF_TOOL_SELECTOR_FREE = 6,
};

/*
 * Enumeration of the different brushes
 * for the paint mode
 */

 enum GMPF_Brush {
    CIRCULAR = 0,
    SQUARE = 1,
 };

/*
 * Enumeration of the different possible action for the user
 * (INVALID, MOVE, MODIF_IMAGE, CHANGE_NAME, DELETE, ADD)
 */
enum GMPF_Action {
    GMPF_ACTION_INCORECT = 0,
    GMPF_ACTION_MOVE_UP = 1,
    GMPF_ACTION_MOVE_DOWN = 2,
    GMPF_ACTION_MODIF_IMAGE = 3,
    GMPF_ACTION_CHANGE_NAME = 4,
    GMPF_ACTION_DELETE = 5,
    GMPF_ACTION_ADD = 6
};


struct savebuf {
    GMPF_Action action;
    int layer; /* Peut-etre mettre des id uniques aux calques.*/
};


/*
 * Structure to store the passed action of the user
 * (buffer, begin, end, size & pos properties)
 */
struct GMPF_Buffer {
    FILE *elmt[BUFFER_SIZE];
    unsigned long long begin;
    unsigned long long end;
    unsigned long long size;
    unsigned long long pos;
};


struct GMPF_List {
    GMPF_List *prev;
    GMPF_List *next;
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
 * A structure to store ar size
 * (width and height properties)
 */
struct GMPF_Size {
    int h, w;
};


/*
* A structure to store the selection
* of the user
*/
struct GMPF_Selection {
    cairo_surface_t *surface;
    GMPF_Size size;
    GMPF_Pos pos;
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
    unsigned char R, G, B, A;
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

    char name[51];
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
 * A brush type (GMPF_Brush): to store the current brush type
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
    GMPF_Brush brush;

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

#endif /* GMPF_STRUCT_H */
