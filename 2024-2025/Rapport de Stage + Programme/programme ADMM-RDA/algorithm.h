#ifndef ALGORITHM_H
#define ALGORITHM_H


// ===================
// Includes
#include "equation.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


// ===================
// Defines



// ===================
// Function to generate a matrix B of combinations of criteria
int main_algo(
    double **database_learning_x, double *database_learning_y, int m_learning, double **database_test_x, double *database_test_y, int m_test, int n, int k, 
    int iteration, int iteration_apprentissage, int iteration_test, 
    double lambda, double gamma, double delta, double tolerance, double rho) ;


#endif // ALGORITHM_H