//
// Created by Kalyan on 9/8/22.
//

#include "gtest/gtest.h"

extern "C" {
#include "../src/fibonacci.h" 
#include "../src/leibniz.h"
#include "../src/monte-carlo.h"
#include "../src/matrix-vector.h"
#include "../src/matrix-matrix.h"
}

TEST(TESTfibonacci, myTest1)
{   
    int a  = fibonacci(10);
    EXPECT_EQ(a, 55); 
} 

TEST(TESTfibonacci, myTest2)
{   
    int a  = fibonacci(10);
    EXPECT_NE(a, 65); 
} 


TEST(TESTleibniz, myTest3)
{   
    double a  = get_pi(3);
    EXPECT_NEAR(a, 3.142,0.000001); 
} 


TEST(TESTleibniz, myTest4)
{   
    double a  = get_pi(3);
    EXPECT_NE(a, 2.0); 
} 


TEST(TESTMontecarlo, myTest3)
{   
    double a  = get_pi(3);
    EXPECT_NEAR(a, 3.142,0.000001); 
} 


TEST(TESTMontecarlo, myTest4)
{   
    double a  = monteCarlo(1000000);
    EXPECT_NEAR(a, 3.14,0.1); 
} 



TEST(TESTMatrixVector, myTest1)
{   
    struct matrixVector* result;

    float array[3] = {14.000000 ,32.000000 ,50.000000};
    char file1[2048];
    getcwd(file1,2048);
    strcat(file1,"/var/test.mm");
    
    char file2[2048];
    getcwd(file2,2048);
    strcat(file2,"/var/testvec.mm");


    result = matrixVectorMultiply(file1, file2);

    // printf("The result of  matrix vector Multiplication \n");
    // for(int i = 0 ; i< result->nrows;i++){
    //     printf("%lf\n", result->A[i]);
    // }

    for (int i = 0; i < result->nrows; ++i) {
        EXPECT_EQ(result->A[i], array[i]) << "Vectors x and y differ at index " << i;
    }
} 


TEST(TESTMatrixMatrix, myTest1)
{   
    struct matrixMatrix* result;

    float array[9] = {30.000000,36.000000,42.000000,66.000000,81.000000,96.000000,102.000000,126.000000,150.000000};
    char file1[2048];
    getcwd(file1,2048);
    strcat(file1,"/var/test.mm");
    
    char file2[2048];
    getcwd(file2,2048);
    strcat(file2,"/var/test2.mm");


    result = matrixMultiply(file1, file2);

    // printf("The result of  matrix vector Multiplication \n");
    // for(int i = 0 ; i< result->nrows;i++){
    //     printf("%lf\n", result->A[i]);
    // }

    for (int i = 0; i < result->nrows; ++i) {
        EXPECT_EQ(result->A[i], array[i]) << "Vectors x and y differ at index " << i;
    }
} 

