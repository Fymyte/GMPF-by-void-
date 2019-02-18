#include "GMPF_LayerMngr.h"


// CODE

/*
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
    layer->size.w = gdk_pixbuf_get_width(layer->image);
    layer->size.h = gdk_pixbuf_get_height(layer->image);
    
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
*/



//
//
// FINISHED PART
//
//



//
// for the GtkFlowBox - interact with Gtk
//

void layermngr_create(GtkFlowBox *flowbox)
{
    /*
        Initialize a new GMPF_LayerMngr and attach it to the flowbox.
    */

    printf("manager 1\n");
    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));

    layermngr_initialization(layermngr);

printf("manager 2\n");

    layermngr->flowbox = flowbox;
    layermngr->display = NULL;

    g_object_set_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME, layermngr);
}

void layermngr_initialization(GMPF_LayerMngr *layermngr)
{
    /*
        Initialize a new GMPF_LayerMngr.
    */
printf("manager init begin\n");
    layermngr->size.h = 0; // maybe change it with parameters
    layermngr->size.w = 0;

    layermngr->nb_layer = 0;
    list_init(&(layermngr->layer_list));
    // add if new variable

    layermngr->image = NULL;
    layermngr->zoomed_image = NULL;

    // don't touch the flowbox and the display again
    // did in the creation
    printf("manager init end\n");
}


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
    if (layermngr->zoomed_image != NULL)
        g_object_unref(layermngr->zoomed_image);

    // reset default values
    layermngr_initialization(layermngr);

}


void layermngr_delete(GtkFlowBox *flowbox)
{
    /*
        Delete the GMPF_LayerMngr attched to the flowbox.
        Use it only when you close the application/GtkWindow.
    */

    // get data and set it to NULL
    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);
    g_object_set_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME, NULL);


    // delete the layermngr
    layermngr_clear(flowbox); // clear the layermngr before delete it
    layermngr->flowbox = NULL; // don't delete the flowbox to keep it
    free(layermngr);
}





//
// for the GMPF_LayerMngr
//


GMPF_Layer * layermngr_get_selected_layer(GtkFlowBox *flowbox)
{
    /*
        Return the GMPF_Layer associated to the selected element in the flowbox.
    */

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

    layer = (GMPF_Layer *) g_object_get_data(G_OBJECT(flowboxchild), LAYER_KEY_NAME);

    return layer;
}


void layermngr_add_new_layer(GtkFlowBox *flowbox)
{
    /*
        Add a GMPF_Layer after the selected element in the flowbox.
        (At first if there is no element in the flowbox)
    */

    GMPF_Layer *newlayer = layer_initialization();
    GMPF_LayerMngr *layermngr =
            (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);

    printf("layer 1\n");
    // add the layer in the list
    if (layermngr->nb_layer == 0)
    {
        list_add_after(&(layermngr->layer_list), &(newlayer->list));
        printf("layer 2A\n");
    }
    else
    {
        GMPF_Layer *prevlayer = layermngr_get_selected_layer(flowbox);
        list_add_after(&(prevlayer->list), &(newlayer->list));
        printf("layer 2B\n");
    }

    /*
    add UIElement to the flowbox
    */
    GtkWidget *image = gtk_image_new();
    printf("layer 3\n");

    // Style of the image
    gtk_widget_set_sensitive (image, TRUE);
    gtk_widget_set_visible (image, TRUE);

    gtk_widget_set_size_request(image, 160, 90); //size
    gtk_widget_set_halign(image, GTK_ALIGN_START); // Alignement
    gtk_widget_set_valign(image, GTK_ALIGN_START);
    gtk_widget_set_margin_top(image, 5); // Margin
    gtk_widget_set_margin_bottom(image, 5);
    gtk_widget_set_margin_start(image, 5);
    gtk_widget_set_margin_end(image, 5);
    printf("layer 4\n");


    //gtk_container_add((GtkContainer *) flowbox, image);
    gtk_flow_box_insert (flowbox, image, layermngr->nb_layer);
    printf("layer 5\n");


    newlayer->UIElement =
        gtk_flow_box_get_child_at_index(flowbox, layermngr->nb_layer);
    g_object_set_data(G_OBJECT(newlayer->UIElement), LAYER_KEY_NAME, newlayer);
    printf("layer 6\n");

    layermngr->nb_layer += 1;
}



void layermngr_delete_selected_layer(GtkFlowBox *flowbox)
{
    /*
        Delete the selected layer.
    */

    GMPF_LayerMngr *layermngr =
        (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);

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
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer));

    layer->pos.x = 0;
    layer->pos.y = 0;

    layer->size.h = 0;
    layer->size.w = 0;

    layer->image = NULL;

    list_init(&(layer->list));

    layer->UIElement = NULL;

    return layer;
}


void layer_delete(GMPF_Layer *layer)
{
    list_remove(&(layer->list));

    // remove and free the GtkFlowBoxChild from the GtkFlowBox
    gtk_widget_destroy((GtkWidget *) layer->UIElement);
    // free the pixbuf
    g_object_unref(layer->image);

    free(layer);
}




//
// for Operations on GMPF_Layer
//
void layer_get_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel)
{
    // TODO
}

void layer_put_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel)
{
    // TODO
}

void layer_rotation(GMPF_Layer *layer, double rad_angle)
{
    // TODO
}

void layer_rotation_right(GMPF_Layer *layer)
{
    // TODO
}

void layer_rotation_left(GMPF_Layer *layer)
{
    // TODO
}
























