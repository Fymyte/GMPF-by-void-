#include "GMPF_LayerMngr.h"

extern SGlobalData G_user_data;

// DEFINE
#define IS_NOT_IN_LAYER(size, pos) \
       (pos->x < 0 || pos->x >= size.w || \
        pos->y < 0 || pos->y >= size.h )


/*****************************Project Info Code********************************/

GMPF_ProjectInfo *GMPF_project_info_init(GtkFlowBox *flowbox)
{
    GMPF_ProjectInfo *info = malloc (sizeof(GMPF_ProjectInfo));
    if (!info)
        return NULL;
    info->filename = NULL;
    info->size.w = 0;
    info->size.h = 0;
    info->scale.x = 1;
    info->scale.y = 1;
    g_object_set_data(G_OBJECT(flowbox), PROJECT_INFO_KEY_NAME, info);
    return info;
}


void GMPF_project_info_destroy(GtkFlowBox *flowbox)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    if (!info)
        return;
    if (info->filename)
        free(info->filename);
    free(info);
    g_object_set_data(G_OBJECT(flowbox), PROJECT_INFO_KEY_NAME, NULL);
}


void GMPF_project_info_reset(GtkFlowBox *flowbox)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    if (!info)
    { PRINTERR("Unable to get Project Info"); return; }

    if (info->filename)
        free(info->filename);
    info->filename = NULL;
    info->size.w = 0;
    info->size.h = 0;
    info->scale.x = 1;
    info->scale.y = 1;
}


GMPF_ProjectInfo *GMPF_project_info_get_project_info(GtkFlowBox *flowbox)
{
    return g_object_get_data(G_OBJECT(flowbox), PROJECT_INFO_KEY_NAME);
}


GMPF_ProjectInfo *GMPF_project_info_set_project_info(GtkFlowBox       *flowbox,
                                                     GMPF_ProjectInfo *info)
{
    GMPF_ProjectInfo *prev_info = GMPF_project_info_get_project_info(flowbox);
    g_object_set_data(G_OBJECT(flowbox), PROJECT_INFO_KEY_NAME, info);
    return prev_info;
}


char *GMPF_project_info_get_filename(GtkFlowBox *flowbox)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    if (!info)
        return NULL;
    return info->filename;
}


bool GMPF_project_info_set_filename(GtkFlowBox *flowbox, const char *filename)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    if (!info)
        return TRUE;

    info->filename = realloc(info->filename, sizeof(char) * (strlen(filename) + 1));
    info->filename = strcpy(info->filename, filename);

    return FALSE;
}


GMPF_Size *GMPF_project_info_get_size(GtkFlowBox *flowbox)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    return info ? &info->size : NULL;
}


bool GMPF_project_info_set_size(GtkFlowBox *flowbox,
                               GMPF_Size   size)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    if (!info)
        return TRUE;
    info->size = size;
    return FALSE;
}


GMPF_Scale *GMPF_project_info_get_scale(GtkFlowBox *flowbox)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    return info ? &info->scale : NULL;
}


bool GMPF_project_info_set_scale(GtkFlowBox *flowbox,
                                 GMPF_Scale  scale)
{
    GMPF_ProjectInfo *info = GMPF_project_info_get_project_info(flowbox);
    if (!info)
        return TRUE;
    info->scale = scale;
    return FALSE;
}

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


void GMPF_selection_reset(GtkFlowBox *flowbox)
{
    GMPF_Selection *selection = GMPF_selection_get_selection(flowbox);
    if (!selection)
        return;
    while (cairo_surface_get_reference_count(selection->surface))
        cairo_surface_destroy(selection->surface);

    selection->surface = NULL;
    selection->size.w = 0;
    selection->size.h = 0;
    selection->pos.x = 0;
    selection->pos.y = 0;
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


/*
 * PURPOSE : Copy the Selection of Layer inside of the Selection
 *  PARAMS : GtkFlowBox *flowbox - The flowbox with contain the Selection
 *           GMPF_Layer *layer - The Layer to copy the content
 * RETURNS : char - 0 if there is no error, else 1
 *   NOTES : Do nothing if there is no Selection on Layer
 */
char GMPF_selection_copy(GtkFlowBox *flowbox,
                         GMPF_Layer *layer,
                         GMPF_Pos    pos)
{
    GMPF_Size size = *GMPF_selection_get_size(flowbox);
    if (!size.w || !size.h)
    { D_PRINT("No selected surface", NULL); return 1; }

    // GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);
    D_PRINT("pos: (%ld, %ld)", pos.x, pos.y);

    cairo_surface_t *surf = GMPF_selection_get_surface(flowbox);
    if (surf)
        cairo_surface_destroy(surf);

    cairo_surface_t *new_surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                                            size.w/layer->scale_factor.x,
                                                            size.h/layer->scale_factor.y);
    cairo_t *cr = cairo_create(new_surf);
    cairo_set_source_surface(cr, layer->surface, pos.x, pos.y);
    cairo_paint(cr);
    cairo_destroy(cr);
    while (cairo_surface_get_reference_count(new_surf) < 5)
        cairo_surface_reference(new_surf);
    GMPF_selection_set_surface(flowbox, new_surf);

    return 0;
}


/*
 * PURPOSE : Paste the content of the Selection on the Layer's surface
 *  PARAMS : GtkFlowBox *flowbox - The flowbow witch contain the Selection
 *           GMPF_Layer *layer - The Layer on witch to paste the Selection
 *           GMPF_Pos    pos - The position at witch to paste
 * RETURNS : char - 0 if there were no error, else 1
 *   NOTES : Do nothing if there is no surface in the Selection
 */
char GMPF_selection_paste(GtkFlowBox *flowbox,
                          GMPF_Layer *layer,
                          GMPF_Pos    pos)
{
    cairo_surface_t *surface = GMPF_selection_get_surface(flowbox);
    if (!surface)
    { D_PRINT("No selected surface", NULL); return 1; }

    layer->cr = cairo_create(layer->surface);

    while (cairo_surface_get_reference_count(surface) < 3)
        cairo_surface_reference(surface);
    cairo_set_source_surface(layer->cr, surface, pos.x, pos.y);
    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, layer);
    cairo_paint(layer->cr);
    cairo_destroy(layer->cr);
    REFRESH_IMAGE(layer);

    return 0;
}


char GMPF_selection_cut(GtkFlowBox *flowbox,
                        GMPF_Layer *layer,
                        GMPF_Pos    pos)
{
    if (GMPF_selection_copy(flowbox, layer, pos))
    { D_PRINT("Unable to copy before cut", NULL); return 1; }

    cairo_surface_t *surface = GMPF_selection_get_surface(flowbox);
    if (!surface)
    { D_PRINT("No selected surface", NULL); return 1; }

    layer->cr = cairo_create(layer->surface);

    while (cairo_surface_get_reference_count(surface) < 3)
        cairo_surface_reference(surface);

    // GMPF_Pos pos = *GMPF_selection_get_pos(flowbox);
    GMPF_Size size = *GMPF_selection_get_size(flowbox);

    cairo_set_source_rgba(layer->cr, 0, 0, 0, 0);
    cairo_set_operator(layer->cr, CAIRO_OPERATOR_SOURCE);
    cairo_rectangle(layer->cr, -pos.x, -pos.y, size.w/layer->scale_factor.x,
                                               size.h/layer->scale_factor.y);
    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, layer);
    cairo_fill(layer->cr);
    cairo_destroy(layer->cr);
    REFRESH_IMAGE(layer);

    return 0;
}


char GMPF_selection_delete(GtkFlowBox *flowbox,
                           GMPF_Layer *layer,
                           GMPF_Pos    pos)
{
    layer->cr = cairo_create(layer->surface);

    GMPF_Size size = *GMPF_selection_get_size(flowbox);

    D_PRINT("pos: (%li, %li), size: %li*%li", pos.x, pos.y, size.w, size.h);
    cairo_set_source_rgba(layer->cr, 0, 0, 0, 0);
    cairo_set_operator(layer->cr, CAIRO_OPERATOR_SOURCE);
    cairo_rectangle(layer->cr, pos.x, pos.y, size.w/layer->scale_factor.x,
                                             size.h/layer->scale_factor.y);
    GMPF_buffer_add(flowbox, GMPF_ACTION_MODIF_IMAGE, layer);
    cairo_fill(layer->cr);
    cairo_destroy(layer->cr);
    REFRESH_IMAGE(layer);

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
    if (!state)
    { GMPF_auto_save_project(flowbox); }
    else
    {
        char *project_filename = GMPF_project_info_get_filename(flowbox);
        if (!project_filename)
            return;

        char *filename = malloc(sizeof(char) * (strlen(project_filename) + 2));
        sprintf(filename, "%s~", project_filename);
        if (remove (filename))
        { D_PRINT("Unable to remove file", NULL); }
        free(filename);
    }
    saved_state->state = state;
}


/*
 * Set the state of the SavedState attached to the flowbox to "state"
 */
// void GMPF_saved_state_set_is_saved_filename(GtkFlowBox *flowbox,
//                                            int         state)
// {
//     GMPF_SavedState *saved_state = GMPF_saved_state_get_saved_state(flowbox);
//     if (!state)
//     { GMPF_auto_save_project(flowbox); }
//     else
//     {
//         char *filename2 = malloc(sizeof(char) * (strlen(filename) + 2));
//         sprintf(filename2, "%s~", filename);
//         if (remove (filename2))
//         { D_PRINT("Unable to remove file", NULL); }
//         free(filename2);
//     }
//     saved_state->state = state;
// }

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
    layermngr->pos.x = -1;
    layermngr->pos.y = -1;

    layermngr->nb_layer = 0;
    list_init(&(layermngr->layer_list));

    layermngr->brush_size = 4;
    layermngr->brush = CIRCULAR;

    layermngr->image = NULL;

    layermngr->surface = NULL;
}


/*
 * Clear all layer contained in the list of layer
 * (Reset all variables to there default value)
 */
void layermngr_clear(GtkFlowBox *flowbox)
{
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

    // reset default values
    layermngr_initialization(layermngr);
    GMPF_buffer_destroy(flowbox);
    GMPF_buffer_init(flowbox);
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
        GMPF_buffer_add(flowbox, GMPF_ACTION_MOVE_UP, layer);
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
        GMPF_buffer_add(flowbox, GMPF_ACTION_MOVE_DOWN, layer);
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
    { PRINTERR("No flowbox"); return NULL; }

    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);

    if (layermngr->nb_layer == 0)
    {  D_PRINT("No data stored", NULL); return NULL; }

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
    INIT_LAYER_UI(newlayer, image, grid);

    int insertpos;
    // add the layer in the list

    list_append(&(layermngr->layer_list), &(newlayer->list));
    insertpos = 0;

    gtk_flow_box_insert (flowbox, grid, insertpos);

    newlayer->UIElement =
        gtk_flow_box_get_child_at_index(flowbox, insertpos);
    g_object_set_data(G_OBJECT(newlayer->UIElement), LAYER_KEY_NAME, newlayer);

    layermngr->nb_layer += 1;

    newlayer->UIIcon = (GtkGrid *) grid;
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
            GMPF_buffer_add(flowbox, GMPF_ACTION_DELETE, layer);
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

    for (size_t i = 0; i < 51; layer->name[i] = '\0', i++);
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
    INIT_LAYER_UI(layer, image, grid);

    gtk_flow_box_insert (flowbox, grid, insertpos);

    layer->UIElement = gtk_flow_box_get_child_at_index(flowbox, insertpos);
    g_object_set_data(G_OBJECT(layer->UIElement), LAYER_KEY_NAME, layer);
    layer->UIIcon = (GtkGrid *) grid;
    layer_icon_refresh(layer);
}


GMPF_Layer *layer_get_at_pos(GtkFlowBox *flowbox,
                             int         pos)
{
    GtkFlowBoxChild *child = gtk_flow_box_get_child_at_index(flowbox, pos);
    if (!child)
    {
        PRINTERR("No child at this pos");
        return NULL;
    }
    return (GMPF_Layer *) g_object_get_data(G_OBJECT(child), LAYER_KEY_NAME);
}






char layer_set_name(GMPF_Layer *layer,
                   char       *name)
{
    if (!layer)
    {
        PRINTERR("Unable to get layer");
        return 1;
    }
    size_t i = 0;

    for(; i < 50 && *name != '\0'; i++, name++)
        layer->name[i] = *name;

    for(; i <= 50; i++)
        layer->name[i] = '\0';

    return 0;
}


char layer_set_image(GtkFlowBox *flowbox,
                     int         pos,
                     GdkPixbuf  *image)
{
    GMPF_Layer *layer = layer_get_at_pos(flowbox, pos);
    if (!layer)
    {
        PRINTERR("Unable to get layer");
        return 1;
    }
    layer->image = image;
    layer->surface = gdk_cairo_surface_create_from_pixbuf(layer->image, 0, NULL);
    layer_icon_refresh(layer);
    return 0;
}


/*
 * Refresh the Layer's image displayed in the list of Layer
 */
void layer_icon_refresh(GMPF_Layer *layer)
{
    if (!layer->UIIcon)
        return;
    GtkGrid *grid = layer->UIIcon;
    GtkWidget *label = gtk_grid_get_child_at(grid, 1, 1);
    GtkImage *UIImage = (GtkImage *)gtk_grid_get_child_at(grid, 0, 0);
    gtk_label_set_text((GtkLabel *)label, !layer->name[0] ? "(Untitled)":layer->name);
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
    layer->icon = gdk_pixbuf_composite_color_simple(layer->image, finalw, finalh,
                    GDK_INTERP_BILINEAR, 255, 4, 0xFF323232, 0xFF777777);

    gtk_image_set_from_pixbuf(UIImage, layer->icon);
    D_PRINT("final: %i, %i", finalw, finalh);
    gtk_widget_set_size_request(GTK_WIDGET(UIImage), finalw, finalh);
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
