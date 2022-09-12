//
// Created by Kalyan on 9/8/22.
//

#include "gtest/gtest.h"

extern "C" {
#include "../src/fibonacci.h" 
#include "../src/leibniz.h"
#include "../src/monte-carlo.h"
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
