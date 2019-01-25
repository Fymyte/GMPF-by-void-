#include "layer_operations.h"












// TODO: NEED TO BE TESTED
int IsInLayer(GMPF_Layer *layer, int x, int y) {
    return x >= 0 && y >= 0 && x < layer->img_size.w && y < layer->img_size.h;
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
}

GdkPixbuf * LayerRotation (GMPF_Layer *layer, double rad_angle) {
    GdkPixbuf *img = layer->image;

    int src_sw = src->w >> 1; // src_sw = src semi width
    int src_sh = src->h >> 1;

    double cosVal = cos(rad_angle);
    double sinVal = sin(rad_angle);


    int dst_w, dst_h;

    myRotation_new_img_size(&dst_w, &dst_h, src_sw, src_sh, cosVal, sinVal);

    GdkPixbuf *dst =
            gdk_pixbuf_new (GdkColorspace colorspace, TRUE,
                int bits_per_sample, dst_w, dst_h); // need to update


    int dst_sw = dst_w / 2;
    int dst_sh = dst_h / 2;

    int x, y, new_i, new_j;

    for (int i = -dst_sw; i < dst_sw; i++)
    {
        for (int j = -dst_sh; j < dst_sh; j++)
        {
            new_i = dst_sw + i;
            new_j = dst_sh + j;
            x = (int) (cosVal * i + sinVal * j);
            y = (int) (-sinVal * i + cosVal * j);
            if (MovePixelBtwSurface(src, src_sw + x, src_sh + y,
                    dst, new_i, new_j))
            {
                SetPixelRGB(dst, new_i, new_j, 255, 255, 255); // need to change
            }
        }
    }

    return dst;
}



// TODO: move from SDL to GDK
void GetPixelRGB(GMPF_Layer *img, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
    
}



// TODO: move from SDL to GDK
void SetPixelRGB(GMPF_Layer *img, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    
}



// TODO: move from SDL to GDK
void MovePixelBtwLayer(GMPF_Layer *src, int sx, int sy, 
        GMPF_Layer *dst, int dx, int dy) {
    int test = IsInSurface(src, sx, sy);
    int test2 = IsInSurface(dst, dx, dy);
    if (test && test2)
    {
        // don't use GetPixelRGB / SetPixelRGB for not test IsInSurface twice
        Uint8 r, g, b;
        Uint32 pixel = getpixel(src, sx, sy);
        SDL_GetRGB(pixel, src->format, &r, &g, &b);
        pixel = SDL_MapRGB(dst->format, r, g, b);
        putpixel(dst, dx, dy, pixel);
    }
}


// TODO: move from SDL to GDK
GMPF_Pixel * LayerGetPixel(GdkPixbuf *surface, unsigned x, unsigned y) {
    
}


// inspired by the function founded on GNOME DEVELOPER website
void LayerPutPixel(GMPF_Layer *layer, unsigned x, unsigned y, GMPF_Pixel *pixel) {
    GdkPixbuf *pixbuf = layer->image;
    int rowstride, n_channels;
    guchar *pixels, *p;

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);

    g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    g_assert (n_channels == 4);

    g_assert (x >= 0 && x < layer->img_size.w);
    g_assert (y >= 0 && y < layer->img_size.h);

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + (x << 2);
        // pixels + y * rowstride + x * nb_channels
    p[0] = pixel->R;
    p[1] = pixel->G;
    p[2] = pixel->B;
    p[3] = pixel->A;
}










