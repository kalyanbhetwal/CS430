#include <stdio.h>
#include <stdlib.h>
#include "../lib/mmio.h"
#include "../include/matrix-matrix.h"

struct matrixMatrix* readMatrix(char* file1)
{
    struct matrixMatrix* m1;
    m1 = malloc( sizeof(struct matrixMatrix));
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int i;

    if ((f = fopen(file1, "r")) == NULL) {
            exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }


    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of array matrix .... */

    if ((ret_code = mm_read_mtx_array_size(f, &(m1->nrows), &(m1->ncolumns))) !=0)
        exit(1);
    /* reseve memory for matrices */
    int num_elem = m1->nrows * m1->ncolumns;
    (m1->A) = (double *) malloc(num_elem * sizeof(double));
    double *val = (double *) malloc(num_elem * sizeof(double));


    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */
    for (i=0; i< num_elem; i++)
    {
        fscanf(f, "%lf\n", &(m1->A)[i]);
    }
    return m1;
    // if (f !=stdin) fclose(f);
   
    // return m1;
    // /************************/
    // /* now write out matrix */
    // /************************/

    // mm_write_banner(stdout, matcode);
    // mm_write_mtx_crd_size(stdout, M, N, nz);
    // for (i=0; i<nz; i++)
    //     fprintf(stdout, "%d %d %20.19g\n", I[i]+1, J[i]+1, val[i]);

}

struct matrixMatrix*  matrixMultiply(char* file1 , char* file2){

    struct matrixMatrix* res;
    res = malloc(sizeof(struct matrixMatrix));
    struct matrixMatrix* m1;
    m1 = malloc( sizeof(struct matrixMatrix));
    m1 = readMatrix(file1);
    struct matrixMatrix* m2;
    m2 = malloc( sizeof(struct matrixMatrix));
    m2 = readMatrix(file2);

  
  
    //double* result;
    res->A = malloc( m1->nrows*m2->ncolumns * sizeof(double));
    int counter=0;
    for(int i=0; i < m1->nrows;i++){
        for(int j=0; j< m2->ncolumns ;j++){
        double result1 = 0;
        for(int k = 0;k <m1-> nrows ;k++){
           result1+=m1->A[i*m1->ncolumns+k]*m2->A[j+k*m2->ncolumns]; 
        }
       res->A[counter] = result1;
       counter++;
      // printf("%lf\n", res->A[i]);
    }
    }
    res->nrows = counter;
    res->ncolumns = m2->ncolumns;
    return res;
}
