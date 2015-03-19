#include "matrixhelpers.h"

double** MatrixHelpers::MakeMatrix(int rows, int cols)
{
    double ** result = new double*[rows];
    for (int i = 0; i < rows; ++i)
        result[i] = new double[cols];
    return result;
}

void MatrixHelpers::ShowVector(double* vector, int decimals, bool blankLine)
{

}


void MatrixHelpers::ShowMatrix(double** matrix, int numRows, int decimals)
{

}
