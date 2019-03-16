#include "GMPF_LayerMngr.h"


// DEFINE
#define IS_NOT_IN_LAYER(size, pos) \
       (pos->x < 0 || pos->x >= size.w || \
        pos->y < 0 || pos->y >= size.h )


// CODE




//
// for the GtkFlowBox - interact with Gtk
//

void layermngr_create(GtkFlowBox *flowbox)
{
    /*
        Initialize a new GMPF_LayerMngr and attach it to the flowbox.
    */

    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));

    layermngr_initialization(layermngr);

    layermngr->flowbox = flowbox;

    g_object_set_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME, layermngr);
}

void layermngr_initialization(GMPF_LayerMngr *layermngr)
{
    /*
        Initialize a new GMPF_LayerMngr.
    */

    layermngr->size.h = 0; // maybe change it with parameters
    layermngr->size.w = 0; // Keep this part, use for max width

    layermngr->nb_layer = 0;
    list_init(&(layermngr->layer_list));
    // add if new variable

    layermngr->image = NULL;
    layermngr->display_image = NULL;

    // don't touch the flowbox and the display again
    // did in the creation
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
    if (layermngr->display_image != NULL)
        g_object_unref(layermngr->display_image);

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

GMPF_LayerMngr *layermngr_get_layermngr(GtkFlowBox *flowbox)
{
    return (GMPF_LayerMngr *) g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME);
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

    if (flowboxchild == NULL)
        return NULL;

    layer = (GMPF_Layer *) g_object_get_data(G_OBJECT(flowboxchild), LAYER_KEY_NAME);

    return layer;
}


void layermngr_add_new_layer(GtkFlowBox *flowbox, const char *filename)
{
    /*
        Add a GMPF_Layer after the selected element in the flowbox.
        (At first if there is no element in the flowbox)
    */

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
        }
        pixbuf_standardized(&(newlayer->image));
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
    gtk_widget_set_sensitive(image, TRUE);
    gtk_widget_set_visible(image, TRUE);

    gtk_widget_set_size_request(image, 160, 90); //size
    gtk_widget_set_halign(image, GTK_ALIGN_START); // Alignement
    gtk_widget_set_valign(image, GTK_ALIGN_START);
    gtk_widget_set_margin_top(image, 5); // Margin
    gtk_widget_set_margin_bottom(image, 5);
    gtk_widget_set_margin_start(image, 5);
    gtk_widget_set_margin_end(image, 5);

    int insertpos;
    // add the layer in the list
    if (layermngr->nb_layer == 0)
    {
        list_add_after(&(layermngr->layer_list), &(newlayer->list));
        insertpos = 0;
    }
    else
    {
        GMPF_Layer *prevlayer = layermngr_get_selected_layer(flowbox);
        if (prevlayer != NULL)
        {
            list_add_after(&(prevlayer->list), &(newlayer->list));
            insertpos = gtk_flow_box_child_get_index(prevlayer->UIElement);
        }
        else
        {
            list_append(&(layermngr->layer_list), &(newlayer->list));
            insertpos = layermngr->nb_layer;
        }
    }

    //gtk_container_add((GtkContainer *) flowbox, image);
    gtk_flow_box_insert (flowbox, image, insertpos);


    newlayer->UIElement =
        gtk_flow_box_get_child_at_index(flowbox, insertpos);
    g_object_set_data(G_OBJECT(newlayer->UIElement), LAYER_KEY_NAME, newlayer);

    layermngr->nb_layer += 1;

    newlayer->UIIcon = (GtkImage *) image;
    layer_icon_refresh(newlayer);

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

        if (layer != NULL)
        {
            layer_delete(layer);
            layermngr->nb_layer -= 1;
        }
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

    layer->isvisible = 1;

    layer->icon = NULL;
    layer->image = NULL;

    layer->surface = NULL;

    list_init(&(layer->list));

    layer->UIIcon = NULL;
    layer->UIElement = NULL;

    return layer;
}


void layer_delete(GMPF_Layer *layer)
{
    list_remove(&(layer->list));

    // remove and free the GtkFlowBoxChild from the GtkFlowBox
    gtk_widget_destroy((GtkWidget *) layer->UIElement);
    // free the pixbuf
    if (layer->icon != NULL)
        g_object_unref(layer->icon);

    if (layer->image != NULL)
        g_object_unref(layer->image);

    if (layer->surface != NULL)
        cairo_surface_destroy (layer->surface);

    free(layer);
}


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

    layer->icon = gdk_pixbuf_scale_simple(layer->image, finalw, finalh,
                         GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(layer->UIIcon, layer->icon);
}



//
// for Operations on GMPF_Layer
//
int layer_get_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel)
{
    GdkPixbuf *pixbuf = layer->image;
    guchar *p;

    if (IS_NOT_IN_LAYER(layer->size, pos))
        return -1;

    int rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    p = gdk_pixbuf_get_pixels (pixbuf);

    p += pos->y * rowstride + (pos->x << 2);
        // pixels + y * rowstride + x * nb_channels
        // nb_channels is always 4.
    pixel->R = p[0];
    pixel->G = p[1];
    pixel->B = p[2];
    pixel->A = p[3];
    return 0;
}

int layer_put_pixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel)
{
    GdkPixbuf *pixbuf = layer->image;
    guchar *pixels, *p;

    if (IS_NOT_IN_LAYER(layer->size, pos))
        return -1;

    int rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + pos->y * rowstride + (pos->x << 2);
        // pixels + y * rowstride + x * nb_channels
    p[0] = pixel->R;
    p[1] = pixel->G;
    p[2] = pixel->B;
    p[3] = pixel->A;
    return 0;
}

/*void layer_rotation(GtkFlowBox *flowbox, double rad_angle)
{
    // TODO
}

void layer_rotation_right(GtkFlowBox *flowbox)
{// TODO: Test it
    GMPF_Layer *layer = layermngr_get_selected_layer(flowbox);
    GMPF_Size newsize = {.w = layer->size.h, .h = layer->size.w};

    GdkPixbuf *pixbuf = layer->image;
    GdkPixbuf *newpixbuf = new_pixbuf_standardized(&newsize);

    GMPF_Pos position = {.x = 0, .y = 0};
    GMPF_Pos newpos = {.x = 0, .y = 0};

    GMPF_Pixel pixel;

    for (; position.x < layer->size.w; position.x++, newpos.y++)
    {
        for (; position.y < layer->size.h; position.y++, newpos.x++)
        {
            layer_get_pixel(pixbuf, &position, &pixel);
            layer_put_pixel(newpixbuf, &newpos, &pixel);
        }
    }

    g_object_unref(layer->image);

    layer->image = newpixbuf;

    layer->size.w = newsize.w;
    layer->size.h = newsize.h;

    // SWAP THE POS TOO
}

void layer_rotation_left(GtkFlowBox *flowbox)
{
    // TODO
}
*/










//
// for GdkPixbuf standardization
//
GdkPixbuf *new_pixbuf_standardized(GMPF_Size *size)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE,
                8, size->w, size->h);
    if (pixbuf_standardized(&pixbuf) == -1)
        return NULL;
    return pixbuf;
}

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
    if (is_error)
        return -1;
    return 0;
}
