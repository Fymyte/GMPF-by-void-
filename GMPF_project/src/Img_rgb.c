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

struct Img_rgb *Assemble_2_Img_rgb(struct Img_rgb *img1, struct Img_rgb *img2)
{
    int width = img1 -> rows;
    int height = img1 -> cols;

    struct Img_rgb *add = init_img_rgb(width, height);
    double r, g, b, a;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            r =(Matrix_IJ(img1->red,i,j) + Matrix_IJ(img2->red,i,j)) / 2;
            g =(Matrix_IJ(img1->green,i,j) + Matrix_IJ(img2->green,i,j)) / 2; 
            b =(Matrix_IJ(img1->blue,i,j) + Matrix_IJ(img2->blue,i,j)) / 2;
            a =(Matrix_IJ(img1->alpha,i,j) + Matrix_IJ(img2->alpha,i,j));
            Matrix_val(add -> red, i, j, r);
            Matrix_val(add -> green, i, j, g);
            Matrix_val(add -> blue, i, j, b);
            Matrix_val(add -> alpha, i, j, a);
        }
    }
    return add;
}
