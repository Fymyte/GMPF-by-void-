#ifndef IMG_RGB_H_
#define IMG_RGB_H_

#include <stdio.h>
#include <stdlib.h>

#include <gtk-3.0/gtk/gtk.h>

#include "Struct.h"
#include "Matrix.h"
#include "pixel_op.h"

struct Img_rgb *init_img_rgb (int rows, int cols);

void img_rgb_zero(struct Img_rgb *img);

void free_img_rgb(struct Img_rgb *img);

void Img_rgb_to_Image(struct _GdkPixbuf *imgPixbuf, struct Img_rgb *img);

struct Img_rgb *Assemble_2_Img_rgb(struct Img_rgb *img1, struct Img_rgb *img2);

#endif
