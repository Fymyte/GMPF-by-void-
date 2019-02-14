#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>

struct Matrix
{
	int rows;
	int cols;
	double *mat;
};

struct Img_rgb
{
    int rows;
    int cols;

    struct Matrix *red;
    struct Matrix *green;
    struct Matrix *blue;
    struct Matrix *alpha;
};

#endif
