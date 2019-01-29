#include "GMPF_LayerOps.h"












// TODO: NEED TO BE TESTED
int IsInLayer(GMPF_Layer *layer, GMPF_Pos *pos) {
    return pos->x >= 0 && pos->y >= 0
        && pos->x < layer->img_size.w && pos->y < layer->img_size.h;
} // 1 = True && 0 = False




// TODO: move from SDL to GDK - ADVANCED
void myRotation_new_img_size(int *width, int *height,
        int s_width, int s_height, double cosValue, double sinValue)
{ // need to remove and add in LayerRotation
    int xVal = (int) fabs(cosValue * (s_width) + sinValue * (-s_height));
    int yVal = (int) fabs(-sinValue * (s_width) + cosValue * (-s_height));

    int xVal2 = (int) fabs(cosValue * (s_width) + sinValue * (s_height));
    int yVal2 = (int) fabs(-sinValue * (s_width) + cosValue * (s_height));
    
    if (xVal < xVal2)
        xVal = xVal2;
    if (yVal < yVal2)
        yVal = yVal2;

    *width = xVal;
    *height = yVal;
}

GdkPixbuf * LayerRotation (GMPF_Layer *src, double rad_angle) {
    GdkPixbuf *img = src->image;

    int src_sw = src->img_size.w >> 1; // src_sw = src semi width
    int src_sh = src->img_size.h >> 1;

    double cosVal = cos(rad_angle);
    double sinVal = sin(rad_angle);


    int dst_w, dst_h;

    myRotation_new_img_size(&dst_w, &dst_h, src_sw, src_sh, cosVal, sinVal);

    GdkPixbuf *dst =
            gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE,
                8, dst_w, dst_h);
            // need to test - need to change w GMPF_Layer init


    int dst_sw = dst_w / 2;
    int dst_sh = dst_h / 2;

    int x, y, new_i, new_j;

    GMPF_Pos src_pos = {.x = 0, .y = 0};
    GMPF_Pos dst_pos = {.x = 0, .y = 0};

    for (int i = -dst_sw; i < dst_sw; i++)
    {
        for (int j = -dst_sh; j < dst_sh; j++)
        {
            new_i = dst_sw + i;
            new_j = dst_sh + j;
            x = (int) (cosVal * i + sinVal * j);
            y = (int) (-sinVal * i + cosVal * j);
            src_pos.x = src_sw + x;
            src_pos.y = src_sh + y;
            dst_pos.x = new_i;
            dst_pos.y = new_j;
            /*if (MovePixelBtwLayer(src, &src_pos, dst, &dst_pos))
            {
                //SetPixelRGB(dst, new_i, new_j, 255, 255, 255); // need to change
            }*/
        }
    }

    return dst;
}






// TODO: move from SDL to GDK
void MovePixelBtwLayer(GMPF_Layer *src, GMPF_Pos *srcpos, 
        GMPF_Layer *dst, GMPF_Pos *dstpos) {
    GMPF_Pixel *pixel = malloc(sizeof(GMPF_Pixel));
    LayerGetPixel(src, srcpos, pixel);
    LayerPutPixel(dst, dstpos, pixel);
    free(pixel);
}


// TODO: move from SDL to GDK
void LayerGetPixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel) {
    GdkPixbuf *pixbuf = layer->image;
    guchar *p;

    int x = pos->x;
    int y = pos->y;

    g_assert (x >= 0 && x < layer->img_size.w); // need to be replaced
    g_assert (y >= 0 && y < layer->img_size.h);

    int rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    p = gdk_pixbuf_get_pixels (pixbuf);

    p += y * rowstride + (x << 2);
        // pixels + y * rowstride + x * nb_channels
        // nb_channels is always 4.
    pixel->R = p[0];
    pixel->G = p[1];
    pixel->B = p[2];
    pixel->A = p[3];
}


// inspired by the function founded on GNOME DEVELOPER website
void LayerPutPixel(GMPF_Layer *layer, GMPF_Pos *pos, GMPF_Pixel *pixel) {
    GdkPixbuf *pixbuf = layer->image;
    //int rowstride, n_channels;
    guchar *pixels, *p;

    //n_channels = gdk_pixbuf_get_n_channels (pixbuf);

    int x = pos->x;
    int y = pos->y;

    //g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    //g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    //g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    //g_assert (n_channels == 4);

    g_assert (x >= 0 && x < layer->img_size.w);
    g_assert (y >= 0 && y < layer->img_size.h);

    int rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + (x << 2);
        // pixels + y * rowstride + x * nb_channels
    p[0] = pixel->R;
    p[1] = pixel->G;
    p[2] = pixel->B;
    p[3] = pixel->A;
}










