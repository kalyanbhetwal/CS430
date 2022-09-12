
typedef struct matrixVector
{
    /* data */
    int nrows;
    int ncolumns;
    double *A;
} matrixVector;

struct matrixVector* readMatrix(char* file1);
struct matrixVector*  matrixMultiply(char* file1 , char* file2);