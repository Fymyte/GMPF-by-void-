#include "layer_operations.h"












// TODO: NEED TO BE TESTED
int IsInSurface(GdkPixbuf *img, int x, int y) {
    if (x >= 0 && y >= 0 && x < img->width && y < img->height)
    {
        return 1; // return True
    }
    return 0; // return False
}




// TODO: move from SDL to GDK
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

GdkPixbuf* myRotation (GdkPixbuf *img, int angle) {
    double rad_angle = (angle / 180.0) * M_PI; //normalize_angle(angle);
    // to have an angle in radians

    int s_img_w = img->w >> 1;
    int s_img_h = img->h >> 1;

    double cosValue = cos(rad_angle);
    double sinValue = sin(rad_angle);


    int r_width, r_height;

    myRotation_new_img_size(&r_width, &r_height, s_img_w, s_img_h, cosValue, sinValue);

    GdkPixbuf *rotation =
            gdk_pixbuf_new (GdkColorspace colorspace,
                TRUE, int bits_per_sample, int width, int height);


    int semi_r_width = r_width / 2;
    int semi_r_height = r_height / 2;

    int x, y, new_i, new_j, isnot_moved;

    for (int i = -semi_r_width; i < semi_r_width; i++)
    {
        for (int j = -semi_r_height; j < semi_r_height; j++)
        {
            new_i = semi_r_width + i;
            new_j = semi_r_height + j;
            x = (int) (cosValue * i + sinValue * j);
            y = (int) (-sinValue * i + cosValue * j);
            isnot_moved = MovePixelBtwSurface(img, s_img_w + x, s_img_h + y,
                    rotation, new_i, new_j);
            if (isnot_moved)
            {
                SetPixelRGB(rotation, new_i, new_j, 255, 255, 255);
            }
        }
    }

    return rotation;
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
        SDL_Surface *dst, int dx, int dy) {
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
static inline
Uint8* pixelref(GdkPixbuf *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

// TODO: move from SDL to GDK
Uint32 GetPixel(GdkPixbuf *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  }
  return 0;
}


// TODO: move from SDL to GDK
void PutPixel(GdkPixbuf *surface, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}










