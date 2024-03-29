#include "Matrix.h"

struct Matrix* init_matrix(Uint64 rows, Uint64 cols)
{
    struct Matrix *m = malloc(sizeof(struct Matrix));
    m -> rows = rows;
    m -> cols = cols;
    m -> mat = malloc(sizeof(double) * rows * cols);
    return m;
}

double Matrix_IJ(struct Matrix *m, Uint64 i, Uint64 j)
{
    return *(m -> mat + i * m -> cols + j);
}

void Matrix_val(struct Matrix *m, Uint64 i, Uint64 j, double val)
{
    *(m -> mat + i * m -> cols + j) = val;
}

void InitializeMatrixZero(struct Matrix *m)
{
    //Initialize the matrix with 0
    for (Uint64 i = 0; i < m -> rows; i++)
    {
        for (Uint64 j = 0; j < m -> cols; j++)
        {
            *(m -> mat + i * m -> cols + j) = 0;
        }
    }
}

void PrintMatrix(struct Matrix *m)
{
    //Print the matrix
	for (Uint64 i = 0; i < m -> rows; i++)
	{
		for (Uint64 j = 0; j < m -> cols; j++)
		{
			printf("%3f ", *(m -> mat + i * m -> cols + j));
		}
	printf("\n");
	}
}

void FreeMatrix(struct Matrix *m)
{
    free(m -> mat);
    free(m);
}
