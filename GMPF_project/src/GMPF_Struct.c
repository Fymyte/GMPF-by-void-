#include "GMPF_Struct.h"


// GMPF_Layer

// return the following GMPF_Layer after layer
GMPF_Layer * free_GMPF_Layer(GMPF_Layer *layer)
{
    GMPF_Layer *layernext = layer->next;

    layer->next = NULL;
    // remove and free the GtkFlowBoxChild from the GtkFlowBox
    gtk_widget_destroy(layer->UIElement);
    // free the pixbuf
    g_object_unref(layer->image);

    free(layer);

    return layernext;
}


// GMPF_LayerMngr
GMPF_LayerMngr * init_GMPF_LayerMngr(GtkFlowBox *UIElement)
{// TODO: test it
    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));
    layermngr->nb_layer = 0;
    layermngr->layer_list = NULL;
    // add if new variable

    layermngr->UIElement = UIElement;

    return layermngr;
}


void free_GMPF_LayerMngr(GMPF_LayerMngr *layermngr)
{// TODO: test it
    while (layermngr->layer_list != NULL) // because it's a chained list
    {
        // remove the element from the layermngr list
        layermngr->layer_list = free_GMPF_Layer(layermngr->layer_list);
    }

    // don't delete the UIElement to keep it in the GtkWindow
    layermngr->UIElement = NULL;
    free(layermngr);
}


