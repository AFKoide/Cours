#include "functions.h"


/****************************** GENERALE *******************************
 * Nom     : factorial
 * Rôle    : Retourne le factoriel d'un nombre
 * Nom     : combinaison
 * Rôle    : Calcul le nombre de combinaison possible pour k-additives
 * Nom     : ProduitScalaire
 * Rôle    : Retourne le produit scalaire 
 **********************************************************************/
long long factorial(int n) {
    long long fact_n = 1, i;

    if (n < 0) return -1;
    for (i = 2; i <= n; i++) {
        fact_n *= i;
    }
    return fact_n;
}

long long combinaison(int n, int p) {
    long long c_n_p;

    if (n < 0 || p > n) return -1;
    c_n_p = factorial(n);
    c_n_p /= factorial(p) * factorial(n - p);
    return c_n_p;
}

double ProduitScalaire(double *x, double *y, int n)
{
    double produit = 0.0 ;
    double *fin = x+n ;
    while( x < fin ) produit += (*x++)*(*y++) ;
    return produit ;
}

long long CalcuTailleGame(int n, int k) {
    long long N = 0; 
    int p;

    for (p = 1; p <= k; p++) {
        N += combinaison(n, p);
    }
    return N;
}





/****************************** ALEATOIRE *******************************
 * Nom     : gaussianRandom
 * Rôle    : Retourne un aléatoire gaussien
 * Nom     : GaussienNorme
 * Rôle    : Retourne une gaussienne bornée et recentrée dans [0, 1]
 * Nom     : RandomPick
 * Rôle    : Tire au hasard deux index et compare les résultats pour ces deux index
 ***********************************************************************/
double gaussianRandom()
{
    double x1, x2, z ;
    
    x1 = rand() / (double)RAND_MAX ;
    x2 = rand() / (double)RAND_MAX ;
    if( x1 < 1e-300 )  x1 = rand() / (double)RAND_MAX ;
    z = sqrt(-2.0*log(x1)) * cos(2.0*M_PI*x2) ;
    return z ;
}

double gaussianNorm()
{
    double x1, x2, z;

    do { x1 = rand() / (double)RAND_MAX; } while (x1 < 1e-300);

    x2 = rand() / (double)RAND_MAX;

    z = sqrt(-2.0 * log(x1)) * cos(2.0 * M_PI * x2);  // gaussienne standard ~ N(0, 1)

    double clamped = fmax(fmin(z, 3.0), -3.0);  
    return (clamped + 3.0) / 6.0;               

}

int RandomPick(double *database_y, int m, int methode, double delta, /*output*/ int *index_1, int *index_2)
{
    if(methode) {
        // Choisir au hasard l'index 1
        *index_1 = rand() % m ;
        // Choisir au hasard l'index 2
        do { *index_2 = rand() % m ; } while( *index_1 == *index_2 ) ;

        if (database_y[*index_2] > database_y[*index_1]) {
            int temp = *index_1;
            *index_1 = *index_2;
            *index_2 = temp;
        }
    }
    else {
        // Choisir un index pair aléatoire
        *index_1 = 2 * (rand() % (m / 2));
        *index_2 = *index_1 + 1;
        // L'index 2 sera l'index impair suivant
        if (*index_2 >= m) {
            *index_2 = 0;
        }
        if (database_y[*index_2] > database_y[*index_1]) {
            int temp = *index_1;
            *index_1 = *index_2;
            *index_2 = temp;
        }
    }

    return EXIT_SUCCESS ;
}





/****************************** MATRICES *******************************
 * Nom     : TransposeMatrix
 * Rôle    : Transpose la matrice en entrée
 * Nom     : MatrixProduct
 * Rôle    : Fait le produit matriciel de deux matrices
 ***********************************************************************/
int TransposeMatrix(double **input, int rows, int cols, double ***output) {
    double **transposed = (double **)malloc(cols * sizeof(double *));
    if (transposed == NULL) {
        fprintf(stderr, "Memory allocation failed for transposed matrix\n");
        *output = NULL;
        return EXIT_FAILURE;
    }
    for (int i = 0; i < cols; i++) {
        transposed[i] = (double *)malloc(rows * sizeof(double));
        if (transposed[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for transposed[%d]\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) free(transposed[j]);
            free(transposed);
            *output = NULL;
            return EXIT_FAILURE;
        }
        for (int j = 0; j < rows; j++) {
            transposed[i][j] = input[j][i];
        }
    }
    *output = transposed;

    return EXIT_SUCCESS;
}


int MatrixProduct(double **A, int rowsA, int colsA, double **B, int rowsB, int colsB, double ***C) {
    if (colsA != rowsB) {
        fprintf(stderr, "Matrix dimensions mismatch for multiplication\n");
        *C = NULL;
        return EXIT_FAILURE;
    }
    double **result = (double **)malloc(rowsA * sizeof(double *));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed for result matrix\n");
        *C = NULL;
        return EXIT_FAILURE;
    }
    for (int i = 0; i < rowsA; i++) {
        result[i] = (double *)malloc(colsB * sizeof(double));
        if (result[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for result[%d]\n", i);
            for (int j = 0; j < i; j++) free(result[j]);
            free(result);
            *C = NULL;
            return EXIT_FAILURE;
        }
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < colsA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    *C = result;
    return EXIT_SUCCESS;
}





/******************************* AUTRES ********************************
 * Nom     : Normalisation_RDA
 * Rôle    : Normalise la base de donnée en utilisant la méthode RDA. 
 *           Retourne un tableau de dimension m x (3 * n)
 ***********************************************************************/
int Normalisation_RDA(double **x, double *y, int m, int n, /*output*/ double ***X_out, int *taille_n)
{
    (void) *y; // Pour l'instant, on s'en sert pas. Pourrait changer dans le futur
        // Step 0) Initialiser tout
    double pourcentage, difference ; 
    double x_min, x_max, x_25, x_50, x_75;
    *taille_n = n * 3;

        // // Step 0.5) Initialiser X
    double **X = (double **)malloc(m * sizeof(double *));
    if (X == NULL) {
        fprintf(stderr, "Erreur allocation X\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < m; i++) {
        X[i] = calloc(*taille_n, sizeof(double));
        if(X[i] == NULL) {
            fprintf(stderr, "Erreur allocation de la ligne %d de X\n", i);
            for (int j = 0; j < i; j++) free(X[j]);
            free(X);
            return EXIT_FAILURE;
        }
    }


    for (int colonne = 0; colonne < n; colonne++) 
    {
            // Step 1) Calculer les seuils 25%, 50% et 75%
        x_min = DBL_MAX;
        x_max = -DBL_MAX;
        for (int ligne = 0; ligne < m; ligne++) {
            x_max = (x[ligne][colonne] > x_max) ? x[ligne][colonne] : x_max;
            x_min = (x[ligne][colonne] < x_min) ? x[ligne][colonne] : x_min;
        }
        difference = x_max - x_min;
        x_25 = (difference / 4) + x_min ; x_50 = (difference / 2) + x_min ; x_75 = (3 * difference / 4) + x_min;


            // Step 2) Calculer les courbes
        for (int ligne = 0; ligne < m; ligne++)
        {
            double x_val = x[ligne][colonne];
            double mu1 = 0.0, mu2 = 0.0, mu3 = 0.0;

            if (x_val <= x_25) {
                mu1 = 1.0;
            }
            else if (x_val >= x_75) {
                mu3 = 1.0;
            }
            else if (x_val == x_50) {
                mu2 = 1.0;
            }
            else if (x_val < x_50) { // x_val > x_25 && x_val < x_50
                double denom = x_50 - x_25;
                if (denom < 1e-8)   denom = 1e-8;

                mu1 = (x_50 - x_val) / denom;
                mu2 = (x_val - x_25) / denom;
            }
            else if (x_val > x_50 && x_val < x_75) {
                double denom = x_75 - x_50;
                if (denom < 1e-8)   denom = 1e-8;

                mu2 = (x_75 - x_val) / denom;
                mu3 = (x_val - x_50) / denom;
            }

            // Affectation finale en une ligne claire
            X[ligne][3 * colonne + 0] = mu1;
            X[ligne][3 * colonne + 1] = mu2;
            X[ligne][3 * colonne + 2] = mu3;
        }
    }
    *X_out = X;  // "Retourner" la matrice via le pointeur de sortie


    return EXIT_SUCCESS;
}