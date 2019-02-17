#include "GMPF_LayerMngr.h"


// CODE

// need to be checked
GMPF_Layer * Layer_CreateFromFile(const char *filename) {
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer));
    GError *error = NULL; // TODO: need to be checked
    GdkPixbuf *pixbuf = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if (pixbuf == NULL)
        err(1, "Uncharged pixbuf\n");
    // NEED TO FREE THE G_ERROR
    layer->image = pixbuf;
    layer->img_size.w = gdk_pixbuf_get_width(layer->image);
    layer->img_size.h = gdk_pixbuf_get_height(layer->image);
    
    int is_error = 0;
    if (gdk_pixbuf_get_colorspace (pixbuf) != GDK_COLORSPACE_RGB)
    {
        printf("Wrong colorspace for pixbuf\n");
        is_error = 1;
    }
    if (gdk_pixbuf_get_bits_per_sample (pixbuf) != 8)
    {
        printf("pixbuf hasn't 8bits per sample\n");
        is_error = 1;
    }
    if (!gdk_pixbuf_get_has_alpha (pixbuf))
    {
        printf("pixbuf as no alpha\n");
        is_error = 1;
    }
    if (gdk_pixbuf_get_n_channels (pixbuf) != 4)
    {
        printf("pixbuf hasn't enough channel\n");
        is_error = 1;
    }
    if (is_error)
        err(1, "There is an error\n");
    
}




//
//
// FINISHED PART
//
//

//
// for the GtkFlowBox - interact with Gtk
//
void layermngr_initialization(GtkFlowBox *flowbox)
{// TODO: test it / ADD THE ERRORS

    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));
    layermngr->nb_layer = 0;
    layermngr->layer_list = NULL;
    // add if new variable

    layermngr->UIElement = flowbox;

    g_object_set_data(flowbox, LAYERMNGR_KEY_NAME, layermngr);

}


void layermngr_clear(GtkFlowBox *flowbox)
{// TODO: test it / ADD THE ERRORS
    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(flowbox, LAYERMNGR_KEY_NAME);

    // delete the layer in the layermngr
    while (layermngr->layer_list != NULL)
    {
        // remove the element from the layermngr list
        layermngr->layer_list = free_GMPF_Layer(layermngr->layer_list);
    }

    layermngr->nb_layer = 0;

}


void layermngr_delete(GtkFlowBox *flowbox)
{// TODO: test it / ADD THE ERRORS


    // get data and set it to NULL
    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(flowbox, LAYERMNGR_KEY_NAME);
    g_object_set_data(flowbox, LAYERMNGR_KEY_NAME, NULL);


    // delete the layermngr
    layermngr_clear(flowbox); // clear the layermngr before delete it
    layermngr->UIElement = NULL; // don't delete the UIElement to keep it
    free(layermngr);
}





//
// for the GMPF_LayerMngr
//



GMPF_Layer * layermngr_get_selected_layer(GtkFlowBox *flowbox)
{
    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(flowbox, LAYERMNGR_KEY_NAME);

    if (layermngr->layer_list == NULL)
        return NULL;

    GMPF_Layer *layer = NULL; // remove NULL when finished
    GList *list = gtk_flow_box_get_selected_children(flowbox);

    GtkFlowBoxChild *flowboxchild =
        (GtkFlowBoxChild *) g_list_nth_data(list, 0);
    // get the first element / there's only one element selected

    g_list_free(list);

    layer = (GMPF_Layer *) g_object_get_data(flowboxchild, LAYER_KEY_NAME);

    return layer;
}


void layermngr_add_new_layer(GtkFlowBox *flowbox)
{
    GMPF_Layer *prevlayer = layermngr_get_selected_layer(flowbox);
    GMPF_Layer *newlayer = layer_initialization();
    GMPF_LayerMngr *layermngr =
            (GMPF_LayerMngr *) g_object_get_data(flowbox, LAYERMNGR_KEY_NAME);

    // add the layer in the list
    if (prevlayer == NULL)
    {
        layermngr->layer_list = newlayer;
    }
    else
    {
        GMPF_Layer *nextlayer = prevlayer->next;
        prevlayer->next = layer;
        layer->prev = prevlayer;
        if (nextlayer != NULL)
            nextlayer->prev = layer;
        layer->next = nextlayer;
    }

    layermngr->nb_layer += 1;

    /*
    add UIElement to the flowbox
    */
    /*GtkWidget *image = gtk_image_new ();





    gtk_container_add ((GtkContainer *) flowbox, image);*/
}



void layermngr_delete_selected_layer(GtkFlowBox *flowbox)
{
    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(flowbox, LAYERMNGR_KEY_NAME);

    if (layermngr->nb_layer != 0)
    {
        GMPF_Layer *layer =
            (GMPF_Layer *) layermngr_get_selected_layer(flowbox);

        layer_delete(layer);
        
        layermngr->nb_layer -= 1;
    }
}



//
// for the GMPF_Layer
//
GMPF_Layer * layer_initialization()
{
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer);

    layer->img_size.h = 0;
    layer->img_size.w = 0;

    layer->image = NULL;

    layer->prev = NULL;
    layer->next = NULL;

    layer->UIElement = NULL;

    return layer;
}


void layer_delete(GMPF_Layer *layer)
{
    GMPF_Layer *layerprev = layer->prev;
    GMPF_Layer *layernext = layer->next;

    if (layerprev != NULL) // need to test
    {
        layerprev->next = layer->next;
    }
    if (layernext != NULL)
    {
        layernext->prev = layer->prev;
    }
    layer->prev = NULL;
    layer->next = NULL;

    // remove and free the GtkFlowBoxChild from the GtkFlowBox
    gtk_widget_destroy((GtkWidget *) layer->UIElement);
    // free the pixbuf
    g_object_unref(layer->image);

    free(layer);

    return layernext;
}























