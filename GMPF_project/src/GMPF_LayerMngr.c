#include "GMPF_LayerMngr.h"


// DEFINE
#define IS_NOT_IN_LAYER(size, pos) \
       (pos->x < 0 || pos->x >= size.w || \
        pos->y < 0 || pos->y >= size.h )

/***************************Selected Surface Code******************************/

/*
 * Init a new Selection structure and attached it to the flowbox
 * (Return: the selection, or NULL if in can't malloc)
 */
GMPF_Selection *GMPF_selection_init(GtkFlowBox *flowbox)
{
    GMPF_Selection *selection = malloc (sizeof(GMPF_Selection));
    if (!selection)
        return NULL;
    selection->surface = NULL;
    selection->size.w = 0;
    selection->size.h = 0;
    selection->pos.x = 0;
    selection->pos.y = 0;
    g_object_set_data(G_OBJECT(flowbox), SELECTION_KEY_NAME, selection);
    return selection;
}


/*
 * Destroy the Selection attached to the flowbox and free it
 */
void GMPF_selection_destroy(GtkFlowBox *flowbox)
{
    GMPF_Selection *selection = GMPF_selection_get_selection(flowbox);
    if (!selection)
        return;
    if (selection->surface)
        cairo_surface_destroy(selection->surface);
    free(selection);
    g_object_set_data(G_OBJECT(flowbox), SELECTION_KEY_NAME, NULL);
}


/*
 * Return the Selection attached to the flowbox
 * (Return: The Selection, or NULL if there is no Selection attached to the
 * flowbox)
 */
GMPF_Selection *GMPF_selection_get_selection(GtkFlowBox *flowbox)
{
    return g_object_get_data(G_OBJECT(flowbox), SELECTION_KEY_NAME);
}


/*
 * Return the previous Selection attached to the flowbox and set the given
 * Selection as the new Selection attached to the flowbox
 * (Return: the previous Selection, or NULL if they were no one before)
 */
GMPF_Selection *GMPF_selection_set_selection(GtkFlowBox     *flowbox,
                                             GMPF_Selection *selection)
{
    GMPF_Selection *prev_selec = g_object_get_data(G_OBJECT(flowbox),
                                                        SELECTION_KEY_NAME);
    g_object_set_data(G_OBJECT(flowbox), SELECTION_KEY_NAME, selection);
    return prev_selec;
}


/*
 * Return the surface of the Selection attached to the flowbox
 * (Return: the surface of the selection, or NULL if Selection is invalid or
 * if there is no surface attached to the selection)
 */
cairo_surface_t *GMPF_selection_get_surface(GtkFlowBox *flowbox)
{
    GMPF_Selection *selection = g_object_get_data(G_OBJECT(flowbox),
                                                        SELECTION_KEY_NAME);
    return selection ? selection->surface : NULL;
}


/*
 * Set the surface of the Selection attached to the flowbox
 * (Be carfull: destroy the older surface)
 * (Return: 0 if there is no error, 1 if it is unable to get the selection)
 */
int GMPF_selection_set_surface(GtkFlowBox      *flowbox,
                               cairo_surface_t *surface)
{
    GMPF_Selection *selection = g_object_get_data(G_OBJECT(flowbox),
                                                        SELECTION_KEY_NAME);
    if (!selection)
        return 1;
    if (selection->surface)
        cairo_surface_destroy(selection->surface);
    selection->surface = surface;
    return 0;
}


/*
 * Return the Position of the Selection attached to the flowbox
 * (Return: the position of the selection, or NULL if Selection is invalid)
 */
GMPF_Pos *GMPF_selection_get_pos(GtkFlowBox *flowbox)
{
    GMPF_Selection *selection = g_object_get_data(G_OBJECT(flowbox),
                                                        SELECTION_KEY_NAME);
    return selection ? &selection->pos : NULL;
}


/*
* Set the Position of the Selection attached to the flowbox
* (Return: 0 if there is no error, 1 if it is unable to get the selection)
*/
int GMPF_selection_set_pos(GtkFlowBox *flowbox,
                           GMPF_Pos    pos)
{
    GMPF_Selection *selection = g_object_get_data(G_OBJECT(flowbox),
                                                        SELECTION_KEY_NAME);
    if (!selection)
        return 1;
    selection->pos = pos;
    return 0;
}


/*
 * Return the Size of the Selection attached to the flowbox
 * (Return: the size of the selection, or NULL if Selection is invalid)
 */
GMPF_Size *GMPF_selection_get_size(GtkFlowBox *flowbox)
{
    GMPF_Selection *selection = g_object_get_data(G_OBJECT(flowbox),
                                                        SELECTION_KEY_NAME);
    return selection ? &selection->size : NULL;
}


/*
* Set the Size of the Selection attached to the flowbox
* (Return: 0 if there is no error, 1 if it is unable to get the selection)
*/
int GMPF_selection_set_size(GtkFlowBox *flowbox,
                           GMPF_Size    size)
{
    GMPF_Selection *selection = g_object_get_data(G_OBJECT(flowbox),
                                                        SELECTION_KEY_NAME);
    if (!selection)
        return 1;
    selection->size = size;
    return 0;
}

/**************************End of Selected Surface*****************************/


/******************************Saved State Code********************************/

/*
 * Init a new SavedState structure and attached it to the flowbox
 * (Return: The SavedState, or NULL if it can't be malloc)
 */
GMPF_SavedState *GMPF_saved_state_init(GtkFlowBox *flowbox)
{
    GMPF_SavedState *state = malloc (sizeof(GMPF_SavedState));
    if (!state)
        return NULL;
    state->state = 1;
    g_object_set_data(G_OBJECT(flowbox), SAVED_STATE_KEY_NAME, state);
    return state;
}


/*
 * Destroy the SavedState attached to the flowbox and free it
 */
void GMPF_saved_state_destroy(GtkFlowBox *flowbox)
{
    GMPF_SavedState *state = GMPF_saved_state_get_saved_state(flowbox);
    if (!state)
        return;
    free(state);
    g_object_set_data(G_OBJECT(flowbox), SAVED_STATE_KEY_NAME, NULL);
}


/*
 * Return the SavedState attached to the flowbox
 * (Return: The SavedState, or NULL if there is no SavedState attached to the
 * flowbox)
 */
GMPF_SavedState *GMPF_saved_state_get_saved_state(GtkFlowBox *flowbox)
{
    return g_object_get_data(G_OBJECT(flowbox), SAVED_STATE_KEY_NAME);
}


/*
 * Return the state of the SavedState attached to the flowbox
 * (Return: The state of the SavedState, or -1 if there is no SavedState
 * attached to the flowbox)
 */
int GMPF_saved_state_get_is_saved(GtkFlowBox *flowbox)
{
    GMPF_SavedState *state = GMPF_saved_state_get_saved_state(flowbox);
    if (!state)
        return -1;
    return state->state;
}


/*
 * Set the state of the SavedState attached to the flowbox to "state"
 */
void GMPF_saved_state_set_is_saved(GtkFlowBox *flowbox,
                                   int         state)
{
    GMPF_SavedState *saved_state = GMPF_saved_state_get_saved_state(flowbox);
    saved_state->state = state;
}

/******************************End of Saved State******************************/


/********************************LayerMngr Code********************************/

/*
 * Create a new LayerMngr, initialize it and attach it to the flowbox
 */
void layermngr_create(GtkFlowBox *flowbox)
{
    SAFE_MALLOC(GMPF_LayerMngr, layermngr);

    layermngr_initialization(layermngr);

    layermngr->flowbox = flowbox;

    g_object_set_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME, layermngr);
}


/*
 * Attach the LayerMngr to the flowbox
 */
void layermngr_set_to_flowbox(GtkFlowBox     *flowbox,
                              GMPF_LayerMngr *layermngr)
{
    layermngr->flowbox = flowbox;
    g_object_set_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME, layermngr);
}


/*
 * Initialize all variables to default values
 */
void layermngr_initialization(GMPF_LayerMngr *layermngr)
{
    layermngr->size.h = 0;
    layermngr->size.w = 0;

    layermngr->pos.x = -1;
    layermngr->pos.y = -1;

    layermngr->filename = NULL;

    layermngr->nb_layer = 0;
    list_init(&(layermngr->layer_list));

    layermngr->brush_size = 4;

    layermngr->image = NULL;
    layermngr->display_image = NULL;

    layermngr->surface = NULL;
}


/*
 * Clear all the layer contained in the list of layer
 * (Reset all variables to there default value)
 */
void layermngr_clear(GtkFlowBox *flowbox)
{
    /*
        Clear the GMPF_LayerMngr attched to the flowbox.
    */

    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);

    // delete the layer in the layermngr
    GMPF_Layer *lay;
    while (layermngr->layer_list.next != NULL)
    {
        // remove the element from the layermngr list
        lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);
        list_remove(layermngr->layer_list.next);
        layer_delete(lay);
    }
    // clear the images
    if (layermngr->image != NULL)
        g_object_unref(layermngr->image);
    if (layermngr->display_image != NULL)
        g_object_unref(layermngr->display_image);

    // reset default values
    layermngr_initialization(layermngr);
}


/*
 * Delete the LayerMngr from the flowbox and free it
 */
void layermngr_delete(GtkFlowBox *flowbox)
{
    // get data and set it to NULL
    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);


    // delete the layermngr
    layermngr_clear(flowbox); // clear the layermngr before delete it
    layermngr->flowbox = NULL; // don't delete the flowbox to keep it
    g_object_set_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME, NULL);
    free(layermngr);
}


/*
 *Move the selected layer one place before in the list of layer
 * (Do nothing if there is no selected Layer)
 */
void layermngr_move_up_selected_layer(GtkFlowBox *flowbox)
{
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    if (layer != NULL && list_move_up(&layer->list))
    {
        //GtkWidget *image = gtk_image_new();
        gint insertpos = gtk_flow_box_child_get_index(layer->UIElement) - 1;
        gtk_widget_destroy((GtkWidget *) layer->UIElement);

        layer_insert_at_pos(layer, flowbox, insertpos);

        gtk_flow_box_select_child(flowbox, layer->UIElement);
    }
}


/*
 * Move the selected layer one place after in the list of layer
 * (Do nothing if there is no selected Layer)
 */
void layermngr_move_down_selected_layer(GtkFlowBox *flowbox)
{
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    if (layer != NULL && list_move_down(&layer->list))
    {
//        //GtkWidget *image = gtk_image_new();
        gint insertpos = gtk_flow_box_child_get_index(layer->UIElement) + 1;
        gtk_widget_destroy((GtkWidget *) layer->UIElement);

        layer_insert_at_pos(layer, flowbox, insertpos);

        gtk_flow_box_select_child(flowbox, layer->UIElement);
    }
}


/*
 * return the LayerMngr attached to the flowbox
 * (Return: the LayerMngr, or NULL if there is no LayerMngr attached to the
 * flowbox)
 */
GMPF_LayerMngr *layermngr_get_layermngr(GtkFlowBox *flowbox)
{
    if (!flowbox)
        return NULL;
    return (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);
}


/*
 * Return the selected layer from the list of layer of the LayerMngr attached
 * to the flowbox
 * (Return: The selected Layer, or NULL if there is no selected Layer)
 */
GMPF_Layer *layermngr_get_selected_layer(GtkFlowBox *flowbox)
{

    if (flowbox == NULL)
        return NULL;

    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);

    if (layermngr->nb_layer == 0)
        return NULL;

    GMPF_Layer *layer = NULL; // remove NULL when finished
    GList *list = gtk_flow_box_get_selected_children(flowbox);

    GtkFlowBoxChild *flowboxchild =
        (GtkFlowBoxChild *) g_list_nth_data(list, 0);
    // get the first element / there's only one element selected

    g_list_free(list);

    if (flowboxchild == NULL)
        return NULL;

    layer = (GMPF_Layer *) g_object_get_data(G_OBJECT(flowboxchild), LAYER_KEY_NAME);

    return layer;
}


/*
 * Add a Layer after the selected Layer in the list of Layer of the LayerMngr
 * attached to the flowbox.
 * (At first positon if there is no element in the flowbox)
 * (Return: The new layer with the associated image, or image set to NULL if
 * unable to load image at filename)
 */
GMPF_Layer *layermngr_add_new_layer(GtkFlowBox *flowbox,
                                    const char *filename)
{
    GMPF_Layer *newlayer = layer_initialization();
    GMPF_LayerMngr *layermngr =
            (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);


    if (filename != NULL)
    {
        GError *gerror = NULL;
        newlayer->image = gdk_pixbuf_new_from_file(filename, &gerror);
        if (gerror)
        {
            printf("error: %s\n", gerror->message);
            g_error_free(gerror);
            newlayer->image = NULL;
        }
        else
        {
            newlayer->filename = (char *)filename;
            pixbuf_standardized(&(newlayer->image));
        }
    }

    if (newlayer->image == NULL)
    {
        GMPF_Size size = {.w=10, .h=10};
        newlayer->image = new_pixbuf_standardized(&size);
    }


    newlayer->surface = gdk_cairo_surface_create_from_pixbuf(newlayer->image, 0, NULL);
    newlayer->size.w = gdk_pixbuf_get_width(newlayer->image);
    newlayer->size.h = gdk_pixbuf_get_height(newlayer->image);


    /*
    add UIElement to the flowbox
    */
    GtkWidget *image = gtk_image_new();

    // Style of the image
    INIT_LAYER_UI(image);

    int insertpos;
    // add the layer in the list

    list_append(&(layermngr->layer_list), &(newlayer->list));
    insertpos = 0;

    gtk_flow_box_insert (flowbox, image, insertpos);

    newlayer->UIElement =
        gtk_flow_box_get_child_at_index(flowbox, insertpos);
    g_object_set_data(G_OBJECT(newlayer->UIElement), LAYER_KEY_NAME, newlayer);

    layermngr->nb_layer += 1;

    newlayer->UIIcon = (GtkImage *) image;
    layer_icon_refresh(newlayer);
    return newlayer;
}


/*
 * Delete the selected Layer in the list of Layer of the LayerMngr attached
 * to the flowbox
 * (Do nothing if no Layer is selected)
 */
void layermngr_delete_selected_layer(GtkFlowBox *flowbox)
{


    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);

    if (layermngr->nb_layer != 0)
    {
        GMPF_Layer *layer =
            (GMPF_Layer *) layermngr_get_selected_layer(flowbox);

        if (layer != NULL)
        {
            layer_delete(layer);
            layermngr->nb_layer -= 1;
        }
    }
}

/***************************End of LayerMngr Code******************************/


/********************************Layer Code************************************/

/*
 * Create a new Layer and init all its variables to there default value
 * (Return: The new layer, or null if unable to malloc)
 */
GMPF_Layer *layer_initialization()
{
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer));
    if (!layer)
    {
        PRINTERR ("Unable to malloc the Layer");
        return NULL;
    }

    layer->name = NULL;
    layer->filename = NULL;

    layer->pos.x = 0;
    layer->pos.y = 0;

    layer->size.h = 0;
    layer->size.w = 0;

    layer->rotate_angle = 0;

    layer->isvisible = 1;

    layer->scale_factor.x = 1;
    layer->scale_factor.y = 1;

    layer->icon = NULL;
    layer->image = NULL;

    layer->cr = NULL;

    // layer->unscaled_surface = NULL;
    layer->surface = NULL;

    list_init(&(layer->list));

    layer->UIIcon = NULL;
    layer->UIElement = NULL;

    return layer;
}


/*
 * Delete the Layer and free it
 */
void layer_delete(GMPF_Layer *layer)
{
    list_remove(&(layer->list));

    // remove and free the GtkFlowBoxChild from the GtkFlowBox
    if (layer->UIElement)
        gtk_widget_destroy((GtkWidget *) layer->UIElement);

    if (layer->icon != NULL)
        g_object_unref(layer->icon);

    if (layer->image != NULL)
        g_object_unref(layer->image);

    if (layer->surface != NULL)
        cairo_surface_destroy (layer->surface);

    free(layer);
}


/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : GMPF_Layer *layer -
 *           GtkFlowBox *flowbox -
 *           int         insertpos -
 * RETURNS : None
 *   NOTES :
 */
void layer_insert_at_pos(GMPF_Layer *layer,
                         GtkFlowBox *flowbox,
                         int         insertpos)
{
    GtkWidget *image = gtk_image_new();
    // Style of the image
    INIT_LAYER_UI(image);

    gtk_flow_box_insert (flowbox, image, insertpos);

    layer->UIElement = gtk_flow_box_get_child_at_index(flowbox, insertpos);
    g_object_set_data(G_OBJECT(layer->UIElement), LAYER_KEY_NAME, layer);
    layer->UIIcon = (GtkImage *) image;
    layer_icon_refresh(layer);
}


/*
 * Refresh the Layer's image displayed in the list of Layer
 */
void layer_icon_refresh(GMPF_Layer *layer)
{
    if (!layer->UIIcon)
        return;
    float ratio1 = layer->size.w / 160.0;
    float ratio2 = layer->size.h / 90.0;
    int finalh = 90;
    int finalw = 160;
    if (ratio1 < ratio2)
        finalw = layer->size.w / ratio2;
    else
        finalh = layer->size.h / ratio1;

    if (layer->icon != NULL)
        g_object_unref(layer->icon);
    layer->icon = gdk_pixbuf_scale_simple(layer->image, finalw, finalh,
                         GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(layer->UIIcon, layer->icon);
}

/******************************End of Layer Code*******************************/


/*
 * Return a new empty pixbuf filled with alpha channel set to zero
 * (Return: the pixbuf, or NULL if it is unable to create it)
 */
GdkPixbuf *new_pixbuf_standardized(GMPF_Size *size)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE,
                8, size->w, size->h);
    if (pixbuf_standardized(&pixbuf))
        return NULL;
    gdk_pixbuf_fill(pixbuf, 0);
    return pixbuf;
}


/*
 * Standerize the pixbuf
 * (With an alpha channel and 8bits per sample)
 * (Return: 0 if there is no error, 1 if there is)
 */
int pixbuf_standardized(GdkPixbuf **pixbuf)
{
    int is_error = 0;
    if (!gdk_pixbuf_get_has_alpha (*pixbuf))
    {
        GdkPixbuf * pix2 = gdk_pixbuf_add_alpha(*pixbuf, FALSE, 0, 0, 0);
        g_object_unref(*pixbuf);
        *pixbuf = pix2;
        if (!gdk_pixbuf_get_has_alpha (*pixbuf))
        {
            printf("pixbuf as no alpha\n");
            is_error = 1;
        }
    }
    if (gdk_pixbuf_get_colorspace (*pixbuf) != GDK_COLORSPACE_RGB)
    {
        printf("Wrong colorspace for pixbuf\n");
        is_error = 1;
    }
    if (gdk_pixbuf_get_bits_per_sample (*pixbuf) != 8)
    {
        printf("pixbuf hasn't 8bits per sample\n");
        is_error = 1;
    }
    if (gdk_pixbuf_get_n_channels (*pixbuf) != 4)
    {
        printf("pixbuf hasn't enough channel\n");
        is_error = 1;
    }

    return is_error;
}
