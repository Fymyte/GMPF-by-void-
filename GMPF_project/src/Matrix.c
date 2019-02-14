#include "Matrix.h"

struct Matrix* init_matrix(int rows, int cols)
{
    struct Matrix *m = malloc(sizeof(struct Matrix));
    m -> rows = rows;
    m -> cols = cols;
    m -> mat = malloc(sizeof(double) * rows * cols);
    return m;
}

double Matrix_IJ(struct Matrix *m, int i, int j)
{
    return *(m -> mat + i * m -> cols + j);
}	

void Matrix_val(struct Matrix *m, int i, int j, double val)
{
    *(m -> mat + i * m -> cols + j) = val;
}

void InitializeMatrixZero(struct Matrix *m)
{
    //Initialize the matrix with 0
    for (int i = 0; i < m -> rows; i++)
    {
        for (int j = 0; j < m -> cols; j++)
        {
            *(m -> mat + i * m -> cols + j) = 50;
        }
    }
}

void PrintMatrix(struct Matrix *m)
{
    //Print the matrix
	for (int i = 0; i < m -> rows; i++)
	{
		for (int j = 0; j < m -> cols; j++)
		{
			printf("%i ",(int) *(m -> mat + i * m -> cols + j));
		}
	printf("\n");
	}
}

void FreeMatrix(struct Matrix *m)
{
    free(m -> mat);
    free(m);
}
