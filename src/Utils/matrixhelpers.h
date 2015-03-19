#ifndef MATRIXHELPERS_H
#define MATRIXHELPERS_H


class MatrixHelpers
{
public:
    static double** MakeMatrix(int rows, int cols);
    static void ShowVector(double* vector, int decimals, bool blankLine);
    static void ShowMatrix(double** matrix, int numRows, int decimals);
};

#endif // MATRIXHELPERS_H
