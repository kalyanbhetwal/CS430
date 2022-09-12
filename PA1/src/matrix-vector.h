//
// Created by Kalyan on 9/12/22.
//

#ifndef PA1_MATRIX_VECTOR_H
#define PA1_MATRIX_VECTOR_H

typedef struct matrixVector
{
    /* data */
    int nrows;
    int ncolumns;
    double *A;
} matrixVector;

struct matrixVector* readMatrix(char* file1);
struct matrixVector*  matrixMultiply(char* file1 , char* file2);

#endif //PA1_MATRIX_VECTOR_H
