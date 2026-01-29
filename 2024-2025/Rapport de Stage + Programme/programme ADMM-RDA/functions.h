#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <float.h>



// ===================
// Define
typedef enum { INDIFFERENCE = 0, STRICT_PREFERENCE = 1 } PreferenceType;
#define BUFFER              fflush(stderr)
#define M_PI		        3.14159265358979323846



// ===================
// Fonctions

// Calcul du nombre de combinaisons (n parmi p).
long long combinaison(int n, int p);

// Calcul de la factorielle de n.
long long factorial(int n);

// Calcul de la taille du jeu (game).
long long CalcuTailleGame(int n, int k);

// Produit scalaire de deux vecteurs.
double ProduitScalaire(double *x, double *y, int n);

// Génération d'un nombre aléatoire selon une loi normale.
double gaussianRandom();

// Génère un nombre aléatoire compris entre [0, 1]
double gaussianNorm();

// Sélection aléatoire d'indices selon une méthode donnée.
int RandomPick(double *database_y, int m, int methode, double delta, int *index_1, int *index_2);

// Transpose une matrice donnée.
int TransposeMatrix(double **input, int rows, int cols, double ***output);

// Fait le produit matriciel de deux matrices données.
int MatrixProduct(double **A, int rowsA, int colsA, double **B, int rowsB, int colsB, double ***C);

//Normalise la base de donnée en utilisant la méthode RDA. 
int Normalisation_RDA(double **x, double *y, int m, int n, /*output*/ double ***X_out, int *taille_n);


#endif // FUNCTIONS_H