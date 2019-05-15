#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

#include "GMPF_Struct.h"

struct Matrix *init_matrix(Uint64 rows, Uint64 columns);

double Matrix_IJ(struct Matrix *m, Uint64 i, Uint64 j);

void Matrix_val(struct Matrix *m, Uint64 i, Uint64 j, double val);

void InitializeMatrixZero(struct Matrix *m);

void PrintMatrix(struct Matrix *m);

void FreeMatrix(struct Matrix *m);

#endif
