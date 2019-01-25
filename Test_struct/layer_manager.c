#include "layer_manager.h"


GMPF_Layer * Layer_CreateFromFile(const char *filename) {
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer));
    GError *error = //need to add GError;
    layer->image = gdk_pixbuf_new_from_file(filename, &error);
    layer->img_size.w = gdk_pixbuf_get_width(layer->image);
    layer->img_size.h = gdk_pixbuf_get_height(layer->image);
}


void Layer_Free(GMPF_Layer *layer) {
    GdkPixbuf *pixbuf = layer->image;
    layer->image = NULL;
    // need to free pixbuf
    free(layer);
}





