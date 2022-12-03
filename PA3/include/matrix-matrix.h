#ifndef PA1_MATRIX_MATRIX_H
#define PA1_MATRIX_MATRIX_H

typedef struct matrixMatrix
{
    /* data */
    int nrows;
    int ncolumns;
    double *A;
} matrixMatrix;

struct matrixMatrix* readMatrix(char* file1);
struct matrixMatrix* readTransposeMatrix(char* file1);
struct matrixMatrix*  matrixMultiply(char* file1 , char* file2);

#endif //PA1_MATRIX_VECTOR_H