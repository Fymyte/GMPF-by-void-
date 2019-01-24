#include "layer_operations.h"












// TODO: NEED TO BE TESTED
int IsInSurface(GdkPixbuf *img, int x, int y) {
    return x >= 0 && y >= 0 && x < img->width && y < img->height;
} // 1 = True && 0 = False




// TODO: move from SDL to GDK - ADVANCED
void myRotation_new_img_size(int *width, int *height,
        int s_width, int s_height, double cosValue, double sinValue)
{
    int xVal = (int) fabs(cosValue * (s_width) + sinValue * (-s_height));
    int yVal = (int) fabs(-sinValue * (s_width) + cosValue * (-s_height));

    int xVal2 = (int) fabs(cosValue * (s_width) + sinValue * (s_height));
    int yVal2 = (int) fabs(-sinValue * (s_width) + cosValue * (s_height));
    
    if (xVal < xVal2)
        xVal = xVal2;
    if (yVal < yVal2)
        yVal = yVal2;
}

GdkPixbuf* myRotation (GdkPixbuf *src, double rad_angle) {
    //double rad_angle = (angle / 180.0) * M_PI; //normalize_angle(angle);
    // to have an angle in radians

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
                SetPixelRGB(dst, new_i, new_j, 255, 255, 255);
            }
        }
    }

    return dst;
}



// TODO: move from SDL to GDK
void GetPixelRGB(GdkPixbuf *img, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
    int test = IsInSurface(img, x, y);
    if (test)
    {
        Uint32 pixel = getpixel(img, x, y);
        SDL_GetRGB(pixel, img->format, r, g, b);
    }
    else
    {
        *r = 0;
        *g = 0;
        *b = 0;
    }
}



// TODO: move from SDL to GDK
void SetPixelRGB(GdkPixbuf *img, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    int test = IsInSurface(img, x, y);
    if (test)
    {
        Uint32 pixel = SDL_MapRGB(img->format, r, g, b);
        putpixel(img, x, y, pixel);
    }
}



// TODO: move from SDL to GDK
void MovePixelBtwSurface(GdkPixbuf *src, int sx, int sy, 
        GdkPixbuf *dst, int dx, int dy) {
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
GMPF_Pixel *GetPixel(GdkPixbuf *surface, unsigned x, unsigned y) {
    
}


// founded on GNOME DEVELOPER website
void PutPixel(GdkPixbuf *surface, unsigned x, unsigned y, GMPF_Pixel *pixel) {
    int width, height, rowstride, n_channels;
    guchar *pixels, *p;

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);

    g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    g_assert (n_channels == 4);

    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);

    g_assert (x >= 0 && x < width);
    g_assert (y >= 0 && y < height);

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * n_channels;
    p[0] = pixel->R;
    p[1] = pixel->G;
    p[2] = pixel->B;
    p[3] = pixel->A;
}










