#include "GMPF_LayerMngr.h"


GMPF_Layer * Layer_CreateFromFile(const char *filename) {
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer));
    GError *error = NULL; // TODO: need to be checked
    GdkPixbuf *pixbuf = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if (pixbuf == NULL)
        err(1, "Uncharged pixbuf\n");
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
    /*if (x < 0 && x >= layer->img_size.w)
    {
        is_error = 1;
    }
    if (y < 0 && y >= layer->img_size.h)
    {
        is_error = 1;
    }*/
    if (is_error)
        err(1, "There is an error\n");
    
}


void Layer_Free(GMPF_Layer *layer) {
    GdkPixbuf *pixbuf = layer->image;
    layer->image = NULL;
    // need to free pixbuf
    free(layer);
}





