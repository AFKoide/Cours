#include "main.h"  // À inclure si tu as des définitions de types ou prototypes




// ------------------------
// Fonction utilitaire : produit scalaire entre deux vecteurs
double ProduitScalaire(double *x, double *y, int n)
{
    double produit = 0.0;
    double *fin = x + n;
    while (x < fin) produit += (*x++) * (*y++);
    return produit;
}

int comparer(Y Choquet, Y MacSum, double difference)
{
    double diff;

    diff = Choquet.y_sup - MacSum.y_sup;
    if(fabs(diff) >= difference) {
        return EXIT_FAILURE;
    }
    
    diff = Choquet.y_inf - MacSum.y_inf;
    if(fabs(diff) >= difference) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}




// ------------------------
// Fonction principale
int main()
{
    // Initialiser variables
    int n = 10;
    double y_inf_MacSum = 0, y_sup_MacSum = 0;
    double y_inf_Choquet = 0, y_sup_Choquet = 0;
    int difference = 1e-35;

    // Allouer mémoire pour les vecteurs
    double *x = (double*)malloc(n * sizeof(double));
    double *var_phi = (double*)malloc(n * sizeof(double));
    double *q_phi = (double*)malloc(2 * n * sizeof(double));
    double *q_phi_inf = q_phi, *q_phi_sup = q_phi + n;


    // Initialiser var_phi avec des poids croissants aléatoires, puis normaliser
    srand(time(NULL));
    double norm = 0.0;
    for (int i = 0; i < n; i++) {
        var_phi[i] = (double)rand() / RAND_MAX;
        norm += var_phi[i] * var_phi[i];
    }
    norm = sqrt(norm);
    if (norm > 0.0) {
        for (int i = 0; i < n; i++) {
            var_phi[i] /= norm;
        }
    }
    printf("var_phi = [");
    for (int i = 0; i < n; i++) {
        printf("%f%s", var_phi[i], (i < n - 1) ? ", " : "");
    }
    printf("]\n");
    printf("----------------------------------\n");

    // Boucle de test
    for (int iter = 0; iter < 100000; iter++) {
        // Générer un vecteur x aléatoire
        for (int i = 0; i < n; i++) {
            x[i] = rand() / (double)RAND_MAX;
        }


        // Calcul de Choquet
        // int MacSum(double *phi, double *x, double *y_inf, double *y_sup, int n)
        MacSu

        // Utiliser CalculPhiX pour MacSum Imprécis (on met k=0 car pas besoin du game, et on n'utilisera que le mode Imprécis).
        CalculPhiX(var_phi, x, q_phi, n, 0, 3);

        y_inf_MacSum = ProduitScalaire(var_phi, q_phi_inf, n);
        y_sup_MacSum = ProduitScalaire(var_phi, q_phi_sup, n);


        Y Choquet = {y_sup_Choquet, y_inf_Choquet}; 
        Y MacSum = {y_sup_MacSum, y_inf_MacSum};

        // Afficher les résultats
        if(comparer(Choquet, MacSum, difference) == EXIT_FAILURE) {
            printf("❌❌❌ Itération %d\n", iter);
            printf("Choquet : y_inf = %f, y_sup = %f\n", Choquet.y_inf, Choquet.y_sup);
            printf("MacSum  : y_inf = %f, y_sup = %f\n", MacSum.y_inf, MacSum.y_sup);
            printf("Différence : %f, %f", fabs(Choquet.y_inf - MacSum.y_inf), fabs(Choquet.y_sup - MacSum.y_sup));
            printf("----------------------------------\n");
            exit(EXIT_FAILURE);
        }
    }

    // Libérer la mémoire
    free(x);
    free(var_phi);
    free(q_phi);

    return 0;
}
