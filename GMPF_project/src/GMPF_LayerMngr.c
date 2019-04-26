#include "GMPF_LayerMngr.h"


// DEFINE
#define IS_NOT_IN_LAYER(size, pos) \
       (pos->x < 0 || pos->x >= size.w || \
        pos->y < 0 || pos->y >= size.h )


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
void GMPF_saved_state_set_is_saved(GtkFlowBox *flowbox, int state)
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
void layermngr_set_to_flowbox(GtkFlowBox *flowbox, GMPF_LayerMngr *layermngr)
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


        GtkWidget *image = gtk_image_new();
        // Style of the image
        INIT_LAYER_UI(image);

        gtk_flow_box_insert (flowbox, image, insertpos);

        layer->UIElement =
            gtk_flow_box_get_child_at_index(flowbox, insertpos);
        g_object_set_data(G_OBJECT(layer->UIElement), LAYER_KEY_NAME, layer);
        layer->UIIcon = (GtkImage *) image;
        layer_icon_refresh(layer);
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


        GtkWidget *image = gtk_image_new();
        // Style of the image
        INIT_LAYER_UI(image);

        gtk_flow_box_insert (flowbox, image, insertpos);


        layer->UIElement =
            gtk_flow_box_get_child_at_index(flowbox, insertpos);
        g_object_set_data(G_OBJECT(layer->UIElement), LAYER_KEY_NAME, layer);
        layer->UIIcon = (GtkImage *) image;
        layer_icon_refresh(layer);
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
GMPF_Layer *layermngr_add_new_layer(GtkFlowBox *flowbox, const char *filename)
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
        PRINTERR;
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
 * Refresh the Layer's image displayed in the list of Layer
 */
void layer_icon_refresh(GMPF_Layer *layer)
{
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
//
// int layer_get_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel)
// {
    //     GdkPixbuf *pixbuf = layer->image;
    //     guchar *p;
    //
    //     if (IS_NOT_IN_LAYER(layer->size, pos))
    //         return -1;
    //
    //     int rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    //     p = gdk_pixbuf_get_pixels (pixbuf);
    //
    //     p += pos->y * rowstride + (pos->x << 2);
    //         // pixels + y * rowstride + x * nb_channels
    //         // nb_channels is always 4.
    //     pixel->R = p[0];
    //     pixel->G = p[1];
    //     pixel->B = p[2];
    //     pixel->A = p[3];
    //     return 0;
    // }
//
// int layer_put_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel)
// {
    //     GdkPixbuf *pixbuf = layer->image;
    //     guchar *pixels, *p;
    //
    //     if (IS_NOT_IN_LAYER(layer->size, pos))
    //         return -1;
    //
    //     int rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    //     pixels = gdk_pixbuf_get_pixels (pixbuf);
    //
    //     p = pixels + pos->y * rowstride + (pos->x << 2);
    //         // pixels + y * rowstride + x * nb_channels
    //     p[0] = pixel->R;
    //     p[1] = pixel->G;
    //     p[2] = pixel->B;
    //     p[3] = pixel->A;
    //     return 0;
        // }
