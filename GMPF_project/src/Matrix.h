#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

#include "Struct.h"

struct Matrix *init_matrix(int rows, int columns);

double Matrix_IJ(struct Matrix *m, int i, int j);

void Matrix_val(struct Matrix *m, int i, int j, double val);

void InitializeMatrixZero(struct Matrix *m);

void PrintMatrix(struct Matrix *m);

void FreeMatrix(struct Matrix *m);

#endif
