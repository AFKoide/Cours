#ifndef EQUATION_H
#define EQUATION_H


// ===================
// Includes
#include "functions.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


// ===================
// Declaration
#define RDA         0
#define ADMM_RDA    1

extern double **B, **BT;
extern double *mu, *z, *g_avg;

typedef struct {
    double erreur_moy;
    int nb_indifference;
    int nb_preference;
    int nb_indifference_vraie;
    int nb_indifference_fausse;
    int nb_preference_vraie;
    int nb_preference_fausse;
    int nb_preference_inverse;
} ResultatsAlgo;


// ===================
// Functions
int Init_ADMMRDA(int n, int indice);

int Learning_ADMMRDA(PreferenceType etat, double *m, double *x, double *x_prime, double y, double y_prime, int indice, int t, double gamma, double lambda, double delta, double rho, int n, int k);

double Error_ADMMRDA(PreferenceType preference, double y, double y_prime, double delta);

int subset_sums(int n, int k, double ***B_out);

int Destroy_ADMMRDA(int n, int indice);

#endif // ALGORITHM_H