#include "saving.h"

//
// private functions declaration
//
char save_layermngr(GMPF_LayerMngr *layermngr, FILE *file)
{
    if (fwrite(layermngr, sizeof(GMPF_LayerMngr), 1, file) != 1)
        return 1;
    return 0;
}

char load_layermngr(GtkFlowBox *flowbox, FILE *file)
{
    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));
    if (layermngr == NULL) { PRINTERR; return 1; }
    if (fread(layermngr, sizeof(GMPF_LayerMngr), 1, file) != 1)
    { PRINTERR; return 1; }
    list_init(&(layermngr->layer_list));

    layermngr->surface = NULL;
    // NEED TO INIT THE CAIRO SURFACE

    layermngr->image = NULL;
    layermngr->display_image = NULL;

    layermngr_set_to_flowbox(flowbox, layermngr);
    return 0;
}

char save_layer(GMPF_Layer *layer, FILE *file)
{
    if (fwrite(layer, sizeof(GMPF_Layer), 1, file) != 1)
     { PRINTERR; return 1; }

    //if (fwrite(layer->image, sizeof(GdkPixbuf), 1, file) != 1)
    //    return 1;
    unsigned int rowstride = (unsigned int) gdk_pixbuf_get_rowstride(layer->image);
    unsigned int length = layer->size.h * rowstride;
    guchar *image = gdk_pixbuf_get_pixels(layer->image);
    if (fwrite(&length, sizeof(unsigned int), 1, file) != 1)
    { PRINTERR; return 1; }
    if (fwrite(image, length, 1, file) != 1) { PRINTERR; return 1; }

    return 0;
}

char load_layer(GMPF_LayerMngr *layermngr, FILE *file)
{
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer));
    if (layer == NULL) { PRINTERR; return 1; }

    GtkFlowBox *flowbox = layermngr->flowbox;
    if (flowbox == NULL) { PRINTERR; return 1; }

    if (fread(layer, sizeof(GMPF_Layer), 1, file) != 1)
    { // restore the layer
        PRINTERR;
        free(layer);
        return 1;
    }
    unsigned int length;
    if (fread(&length, sizeof(unsigned int), 1, file) != 1)
    {
        PRINTERR;
        free(layer);
        return 1;
    }

    unsigned int true_length = ((unsigned int) layer->size.w * ((unsigned int) layer->size.h << 2));
    if (length != true_length)
    { // test if the length of the pixmap is good
        PRINTERR;
        free(layer);
        return 1;
    }

    guchar *data = malloc(length * sizeof(guchar));
    if (fread(data, length, 1, file) != 1)
    {
        PRINTERR;
        free(data);
        free(layer);
        return 1;
    }

    layer->image = gdk_pixbuf_new_from_data (data,
                           GDK_COLORSPACE_RGB, TRUE, 8,
                           layer->size.w, layer->size.h,
                           layer->size.w << 2,
                       NULL, NULL);
    D_PRINT("%p", layer->image);

    layer->surface = gdk_cairo_surface_create_from_pixbuf(layer->image, 0, NULL);

    GtkWidget *image = gtk_image_new();

    // Style of the image
    INIT_LAYER_UI(image);

    /*int insertpos;*/
    // add the layer in the list
    list_append(&(layermngr->layer_list), &(layer->list));
    /*insertpos = 0;*/

    gtk_flow_box_insert (flowbox, image, 0 /*insertpos*/);

    layer->UIElement =
        gtk_flow_box_get_child_at_index(flowbox, 0 /*insertpos*/);
    g_object_set_data(G_OBJECT(layer->UIElement), LAYER_KEY_NAME, layer);

    layer->UIIcon = (GtkImage *) image;

    layer->cr = NULL;
    layer->icon = NULL;
    layer_icon_refresh(layer);


    return 0;
}



//
// MAIN FUNCTIONS
//
char save_project(GtkFlowBox *flowbox, const char *filename)
{
    // write the file / delete it if it already exists
    //FILE *tmpFile = tmpfile(void);

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr == NULL) { PRINTERR; return 1; }

    FILE *file = fopen(filename, "wb"); // write as binary => rb
    if (file == NULL) { PRINTERR; return 1; }

    char err = save_layermngr(layermngr, file);

    if (err) { fclose(file); PRINTERR; return 1; }

    GMPF_List *list = layermngr->layer_list.next;
    GMPF_Layer *lay = container_of(list, GMPF_Layer, list);
    for (int i = 0; i < layermngr->nb_layer; i++)
    {
        err = save_layer(lay, file);
        if (err) { fclose(file); PRINTERR; return 1;}
        lay = container_of(lay->list.next, GMPF_Layer, list);
    }

    long long dbg = 0;
    fwrite(&dbg, sizeof(long long), 1, file);

    fclose(file);
    return 0;
}

char load_project(GtkFlowBox *flowbox, const char *filename)
{
    // only read the file
    FILE *file = fopen(filename, "rb"); // read as binary => rb
    if (file == NULL) { PRINTERR; return 1; }

    if (g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME) != NULL)
        layermngr_delete(flowbox);

    char err = load_layermngr(flowbox, file);
    if (err) { fclose(file); PRINTERR; return 1; }

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr == NULL) { fclose(file); PRINTERR; return 1; }

    for (int i = 0; i < layermngr->nb_layer; i++)
    {
        err = load_layer(layermngr, file);
        if (err) { fclose(file); PRINTERR; return 1; }
    }

    fclose(file);
    return 0;
}

char saving_layer(GtkFlowBox *flowbox, const char *filename)
{
    // write the file / delete it if it already exists
    //FILE *tmpFile = tmpfile(void)

    FILE *file = fopen(filename, "wb"); // write as binary => rb
    if (file == NULL) { PRINTERR; return 1; }

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return 1;
    char err = save_layer(lay, file);
    if (err) { fclose(file); PRINTERR; return 1;}

    long long dbg = 0;
    fwrite(&dbg, sizeof(long long), 1, file);

    fclose(file);
    return 0;
}

char loading_layer(GtkFlowBox *flowbox, const char *filename)
{
    // only read the file
    FILE *file = fopen(filename, "rb"); // read as binary => rb
    if (file == NULL) { PRINTERR; return 1; }

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr == NULL) { fclose(file); PRINTERR; return 1; }

    char err = load_layer(layermngr, file);
    if (err) { fclose(file); PRINTERR; return 1; }

    fclose(file);
    return 0;
}
