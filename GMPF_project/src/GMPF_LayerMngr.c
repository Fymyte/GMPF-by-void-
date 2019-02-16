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




GMPF_Layer * LayerMngr_get_selected_layer(GMPF_LayerMngr *layermngr)
{
    GMPF_Layer *layer = NULL; // remove NULL when finished
    GtkFlowBox *flowbox = layermngr->UIElement;
    GList *list = gtk_flow_box_get_selected_children (flowbox);
    GtkFlowBoxChild *flowboxchild = list->data;
    // there's only one element selected
    g_list_free(list);

    // GET THE LAYER WITH THE FLOWBOXCHILD

    return layer;
}






void LayerMngr_Append_Layer(GMPF_LayerMngr *layermngr)
{

}



void LayerMngr_Delete_Layer(GMPF_LayerMngr *layermngr, int layernum)
{
    if (layernum < 0 || layernum >= layermngr->nb_layer)
    {
        return;
    }
    if (layernum == 0)
    {
        layermngr->layer_list = free_GMPF_Layer(layermngr->layer_list);
        return;
    }
    GMPF_Layer *layer = layermngr->layer_list;
    layernum--;
    while (layernum > 0) // the first element is 0
    {
        layer = layer->next;
        layernum--;
    }
    layer->next = free_GMPF_Layer(layer->next);
    return;
}




























