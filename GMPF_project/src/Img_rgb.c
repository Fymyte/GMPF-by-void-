#include "Img_rgb.h"

struct Img_rgb *init_img_rgb(int rows, int cols)
{
    struct Img_rgb *img = malloc(sizeof(struct Img_rgb));
    img -> rows = rows;
    img -> cols = cols;
    img -> red = init_matrix(rows, cols);
    img -> green = init_matrix(rows, cols);
    img -> blue = init_matrix(rows, cols);
    img -> alpha = init_matrix(rows, cols);
    return img;
}

void img_rgb_zero(struct Img_rgb *img)
{
    InitializeMatrixZero(img -> red);
    InitializeMatrixZero(img -> green);
    InitializeMatrixZero(img -> blue);
}

void free_img_rgb(struct Img_rgb *img)
{
    FreeMatrix(img -> red);
    FreeMatrix(img -> green);
    FreeMatrix(img -> blue);
    free(img);
}

void Img_rgb_to_Image(struct _GdkPixbuf *imgPixbuf, struct Img_rgb *img)
{
    int width = gdk_pixbuf_get_width(imgPixbuf);
    int height = gdk_pixbuf_get_height(imgPixbuf);

    guchar r;
    guchar g;
    guchar b;
    guchar a;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            r = Matrix_IJ(img -> red, i, j);
            g = Matrix_IJ(img -> green, i , j);
            b = Matrix_IJ(img -> blue, i, j);
            a = Matrix_IJ(img -> alpha, i, j);

            put_pixel(imgPixbuf, i, j, r, g, b, a);
        }
    }
}

