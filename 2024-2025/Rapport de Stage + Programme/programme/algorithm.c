#include "algorithm.h"

#define M_PI		        3.14159265358979323846

# define LINEAR             0
# define GAME               1
# define MACSUM             2
# define MACSUM_IMPRECIS    3

#define FORT                1
#define FAIBLE              0

#define BUFFER              fflush(stdout)


int taille_phi; // Variable globale pour la taille nécessaire



double ProduitScalaire(double *x, double *y, int n)
{
    double produit = 0.0 ;
    double *fin = x+n ;
    while( x < fin ) produit += (*x++)*(*y++) ;
    return produit ;
}


double gaussianRandom()
{
    double x1, x2, z ;
    
    x1 = rand() / (double)RAND_MAX ;
    x2 = rand() / (double)RAND_MAX ;
    if( x1 < 1e-300 )  x1 = rand() / (double)RAND_MAX ;
    z = sqrt(-2.0*log(x1)) * cos(2.0*M_PI*x2) ;
    return z ;
}

int RandomPick(double *database_y, int m, int methode, /*output*/ int *index_1, int *index_2)
{
    if(methode) {
        *index_1 = rand() % m ;
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



// --------------------------------------------------------------------------------
// Learning_algorithm : Algorithme utilisé pour mettre à jour les poids.
// --------------------------------------------------------------------------------
// database_learning_x, database_learning_y, var_phi, i+1, n, m_learning, k, iteration_apprentissage, err_memory, beta, delta, gamma, tolerance, use_L1, method, ordre
ResultatsAlgo Algorithm_learning(double **database_learning_x, double *database_learning_y, double *var_phi, int epoch, int n, int m, int k, int iteration_apprentissage, double err_precedente, double beta, double delta, double gamma, double tolerance, int use_L1, int method, int ordre) 
{
    (void) ordre;
    int index1, index2;
    double y, y_prime, y1, y2, *x1, *x2;

    int indifference_vrai;
    int nb_indifference = 0, nb_preference = 0, nb_indifference_vraie = 0, nb_indifference_fausse = 0, nb_preference_vraie = 0, nb_preference_fausse = 0, nb_preference_inverse = 0;

    double *q_phi       = (double *)malloc(taille_phi * sizeof(double));
    double *q_phi_prime = (double *)malloc(taille_phi * sizeof(double));
    if (q_phi == NULL || q_phi_prime == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire pour q_phi ou q_phi_prime. Localisation : Algorithm_learning\n");
        exit(EXIT_FAILURE);
    }


    // Définition du type de pointeur de fonction pour update
    typedef void (*UpdateFunc)( int indifference_vrai, double *var_phi, double *q_phi, double *q_phi_prime, double y, double y_prime, int epoch, double beta, double delta, double gamma, int taille_phi );


    // Sélection du pointeur de fonction update
    UpdateFunc update_func = use_L1 ? L1_Update : L2_Update;


    for (int i = 0; i < iteration_apprentissage; i++) {
        // Sélection aléatoire de deux indices
        RandomPick(database_learning_y, m, 0, &index1, &index2);

        y1 = database_learning_y[index1] ;
        y2 = database_learning_y[index2] ;
        x1 = database_learning_x[index1] ;
        x2 = database_learning_x[index2] ;

        indifference_vrai = (fabs(y1 - y2) <= tolerance) ? 1 : 0 ;
        if(indifference_vrai) nb_indifference++ ; else nb_preference++ ;


        y = 0.0; y_prime = 0.0;
        if(CalculPhiX(var_phi, x1, q_phi, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x dans Algorithm_Learning.\n");
            exit(EXIT_FAILURE);
        }
        if(CalculPhiX(var_phi, x2, q_phi_prime, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x_prime dans Algorithm_Learning.\n");
            exit(EXIT_FAILURE);
        }


        y       = ProduitScalaire(q_phi,var_phi,taille_phi) ;
        y_prime = ProduitScalaire(q_phi_prime,var_phi,taille_phi) ;


        update_func(indifference_vrai, var_phi, q_phi, q_phi_prime, y, y_prime, epoch, beta, delta, gamma, taille_phi);

        // Mise à jour des compteurs de performance
        char indifference_predite = (fabs(y - y_prime) <= delta) ? 1 : 0 ;
        char preference_predite = ((y - y_prime) > delta) ? 1 : 0 ;
        if (indifference_vrai) {
            if (indifference_predite)
                nb_indifference_vraie++;
            else
                nb_indifference_fausse++;
        } 
        else {
            if (preference_predite)
                nb_preference_vraie++;
            else if (y_prime > (y + delta))
                nb_preference_inverse++;
            else 
                nb_preference_fausse++;
        }
    }

    free(q_phi);
    free(q_phi_prime);


    double erreur_moy = Error_mean(database_learning_x, database_learning_y, var_phi, tolerance, delta, 1000, m, n, use_L1, method, k);
    if (erreur_moy > err_precedente) 
        erreur_moy = err_precedente;

    // Affichage de l'erreur moyenne pour cette époque
    if (epoch % 100 == 0) // Affiche toutes les 10 époques
        printf("Époque %d : Nombre indifférences = %d ; Nombre préférences = %d, Erreur moyenne = %.10f\n, Indifférences vraies = %d, Indifférences fausses = %d, Préférences vraies = %d, Préférences fausses = %d, Préférences inversées = %d\n\n",
                epoch, nb_indifference, nb_preference, erreur_moy, nb_indifference_vraie, nb_indifference_fausse, nb_preference_vraie, nb_preference_fausse, nb_preference_inverse);


    ResultatsAlgo result;
    result.erreur_moy = erreur_moy;
    result.nb_indifference = nb_indifference;
    result.nb_preference = nb_preference;
    result.nb_indifference_vraie = nb_indifference_vraie;
    result.nb_indifference_fausse = nb_indifference_fausse;
    result.nb_preference_vraie = nb_preference_vraie;
    result.nb_preference_fausse = nb_preference_fausse;
    result.nb_preference_inverse = nb_preference_inverse;

    return result;
}





double Error_mean(double **database_x, double *database_y, double *var_phi, double tolerance, double delta, int nb_samples, int m, int n, int use_L1, int method, int k) {
//    printf("b\n");
    double sum_error = 0.0;
    int index1, index2;
    double y, y_prime;
    int indifference_vrai;
    double y1, y2, *x1, *x2;

    double *q_phi       = (double *)malloc(taille_phi * sizeof(double));
    double *q_phi_prime = (double *)malloc(taille_phi * sizeof(double));
    if (q_phi == NULL || q_phi_prime == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour q_phi ou q_phi_prime. Localisation : Error_mean\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nb_samples; i++) {
        RandomPick(database_y, m, 0, &index1, &index2);

        x1 = database_x[index1] ;
        y1 = database_y[index1] ;
        x2 = database_x[index2] ;
        y2 = database_y[index2] ;

        indifference_vrai = (fabs(y1 - y2) <= tolerance);

        if(CalculPhiX(var_phi, x1, q_phi, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x dans Error_Mean.\n");
            exit(EXIT_FAILURE);
        }
        if(CalculPhiX(var_phi, x2, q_phi_prime, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x_prime dans Error_Mean.'\n");
            exit(EXIT_FAILURE);
        }

        y       = ProduitScalaire(q_phi,var_phi,taille_phi) ;
        y_prime = ProduitScalaire(q_phi_prime,var_phi,taille_phi) ;

        if (use_L1)
            sum_error += L1_Error(indifference_vrai, y, y_prime, delta);
        else
            sum_error += L2_Error(indifference_vrai, y, y_prime, delta);
    }

    free(q_phi);
    free(q_phi_prime);

    return sum_error / (double) nb_samples;
}



ResultatsAlgo Algorithm_test(double **database_test_x, double *database_test_y, double *var_phi, int n, int m, int k, int iteration_test, double err_precedente, double tolerance, double delta, int method, int use_L1, int ordre) 
{
    (void) ordre;
    int index1, index2;
    double y, y_prime;
    double y1, y2, *x1, *x2;
    double *q_phi       = (double *)malloc(taille_phi * sizeof(double));
    double *q_phi_prime = (double *)malloc(taille_phi * sizeof(double));
    if (q_phi == NULL || q_phi_prime == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour q_phi ou q_phi_prime. Localisation : Algorithm_test\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation des compteurs
    char indifference_vrai ;
    int nb_indifference = 0, nb_preference = 0, nb_indifference_vraie = 0, nb_indifference_fausse = 0, nb_preference_vraie = 0, nb_preference_fausse = 0, nb_preference_inverse = 0;

    for (int i = 0; i < iteration_test; i++) {
        RandomPick(database_test_y, m, 0, &index1, &index2);

        x1 = database_test_x[index1] ;
        y1 = database_test_y[index1] ;
        x2 = database_test_x[index2] ;
        y2 = database_test_y[index2] ;


        indifference_vrai = (fabs(y1 - y2) <= tolerance) ? 1 : 0 ;
        if(indifference_vrai) nb_indifference++ ; else nb_preference++ ;

        
        y = 0.0 ; y_prime = 0.0;

        if(CalculPhiX(var_phi, x1, q_phi, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x dans Algorithm_Learning.\n");
            exit(EXIT_FAILURE);
        }
        if(CalculPhiX(var_phi, x2, q_phi_prime, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x_prime dans Algorithm_Learning.\n");
            exit(EXIT_FAILURE);
        }


        y       = ProduitScalaire(q_phi,var_phi,taille_phi) ;
        y_prime = ProduitScalaire(q_phi_prime,var_phi,taille_phi) ;


        // Mise à jour des compteurs de performance
        char indifference_predite = (fabs(y - y_prime) <= delta) ? 1 : 0 ;
        char preference_predite = ((y - y_prime) > delta) ? 1 : 0 ;
        if (indifference_vrai) {
            if (indifference_predite)
                nb_indifference_vraie++;
            else
                nb_indifference_fausse++;
        } 
        else {
            if (preference_predite)
                nb_preference_vraie++;
            else if (y_prime > (y + delta))
                nb_preference_inverse++;
            else 
                nb_preference_fausse++;
        }
    }
    // Libération de la mémoire
    free(q_phi);
    free(q_phi_prime);

    double erreur_moy = Error_mean(database_test_x, database_test_y, var_phi, tolerance, delta, 1000, m, n, use_L1, method, k);

    // if (erreur_moy > err_precedente) 
    //     erreur_moy = err_precedente;

        // Log des résultats du test
    // err_train,nombre_egalite,nombre_preference,pred_vrai_egalite,pred_fausse_egalite,pred_vrai_preference,pred_fausse_preference

    ResultatsAlgo result;
    result.erreur_moy = erreur_moy;
    result.nb_indifference = nb_indifference;
    result.nb_preference = nb_preference;
    result.nb_indifference_vraie = nb_indifference_vraie;
    result.nb_indifference_fausse = nb_indifference_fausse;
    result.nb_preference_vraie = nb_preference_vraie;
    result.nb_preference_fausse = nb_preference_fausse;
    result.nb_preference_inverse = nb_preference_inverse;

    return result;
}





// ============================================================================================================
// ============================================================================================================
// ============================================================================================================

void Compteurs_Imprecis(ResultatsAlgo *compteurs, int indifference_vrai, int ordre, 
                        double y_inf, double y_sup, double y_prime_inf, double y_prime_sup)
{
    char indifference_predite = 0, preference_predite = 0, preference_inverse = 0;

    if(y_sup < y_inf || y_prime_sup < y_prime_inf) {
        printf("\n\n====== PROBLEME AVEC Y_INF ET Y_SUP ======\n");
        printf("Y_INF = %.10f \t Y_SUP = %.10f\n", y_inf, y_sup);
        printf("Y_PRIME_INF = %.10f \t Y_PRIME_SUP = %.10f\n", y_prime_inf, y_prime_sup);
        printf("Adresse y_inf: %p, y_sup: %p\n", (void*)&y_inf, (void*)&y_sup);
        printf("Adresse y_prime_inf: %p, y_prime_sup: %p\n", (void*)&y_prime_inf, (void*)&y_prime_sup);
        printf("Valeur binaire y_inf: 0x%016llx\n", *(unsigned long long*)&y_inf);
        printf("Valeur binaire y_sup: 0x%016llx\n", *(unsigned long long*)&y_sup);
        printf("Valeur binaire y_prime_inf: 0x%016llx\n", *(unsigned long long*)&y_prime_inf);
        printf("Valeur binaire y_prime_sup: 0x%016llx\n", *(unsigned long long*)&y_prime_sup);
        exit(EXIT_FAILURE);
    }

    // Programme du prof
    switch (ordre)
    {
    case FAIBLE:
        preference_predite   = ((y_inf > y_prime_inf) && (y_sup > y_prime_sup)) ? 1 : 0;
        indifference_predite = (((y_inf - y_prime_inf) * (y_sup - y_prime_sup)) < 0.0) ? 1 : 0;
        preference_inverse   = ((y_inf < y_prime_inf) && (y_sup < y_prime_sup)) ? 1 : 0;
        break;

    case FORT:
        preference_predite   = (y_inf > y_prime_sup) ? 1 : 0;
        indifference_predite = (((y_sup - y_prime_inf) * (y_inf - y_prime_sup)) <= 0.0) ? 1 : 0;
        preference_inverse   = (y_prime_inf > y_sup) ? 1 : 0;
        break;
    }


    if (indifference_vrai) {
        compteurs->nb_indifference++;
        if (indifference_predite)
            compteurs->nb_indifference_vraie++;
        else
            compteurs->nb_indifference_fausse++;
    } 
    else {
        compteurs->nb_preference++;
        if (preference_predite)
            compteurs->nb_preference_vraie++;
        else if (preference_inverse)
            compteurs->nb_preference_inverse++;
        else 
            compteurs->nb_preference_fausse++;
    }
}






ResultatsAlgo Algorithm_Learning_Imprecis(double **database_learning_x, double *database_learning_y, double *var_phi, int epoch, int n, int m, int k, int iteration_apprentissage, double err_precedente, double beta, double delta, double gamma, double tolerance, int use_L1, int method, int ordre) 
{
    (void) delta;
    int index1, index2;
    double y_inf, y_sup, y_prime_inf, y_prime_sup, *x1, *x2, y1, y2;
    ResultatsAlgo resultats = {0, 0, 0, 0, 0, 0, 0, 0};
    

    double *q_phi       = (double *)malloc(taille_phi * 2 * sizeof(double));
    double *q_phi_prime = (double *)malloc(taille_phi * 2 * sizeof(double));
    if (q_phi == NULL || q_phi_prime == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour q_phi ou q_phi_prime. Localisation : Algorithm_Learning_Imprecis\n");
        exit(EXIT_FAILURE);
    }
    double *q_phi_inf = q_phi,             *q_phi_sup = q_phi + taille_phi;
    double *q_phi_prime_inf = q_phi_prime, *q_phi_prime_sup = q_phi_prime + taille_phi; 


    // Définition du type de pointeur de fonction pour update imprecis
    typedef int (*UpdateImprecisFunc)(double *var_phi, double *q_phi_inf, double *q_phi_sup, double *q_phi_prime_inf, double *q_phi_prime_sup, double y_inf, double y_sup, double y_prime_inf, double y_prime_sup, char indifference_vrai, double beta, double gamma, int n, int epoch);

    // Sélection du pointeur de fonction update imprecis selon use_L1 et ordre
    UpdateImprecisFunc update_imprecis_func = NULL;
    if (use_L1) {
        if (ordre == FAIBLE)
            update_imprecis_func = L1_Update_Imprecis_FAIBLE;
        else
            update_imprecis_func = L1_Update_Imprecis_FORT;
    } else {
        if (ordre == FAIBLE)
            update_imprecis_func = L2_Update_Imprecis_FAIBLE;
        else
            update_imprecis_func = L2_Update_Imprecis_FORT;
    }

    if (update_imprecis_func == NULL) {
        fprintf(stderr, "[CRITICAL] update_imprecis_func est NULL !\n");
        exit(EXIT_FAILURE);
    }
    


    for (int i = 0; i < iteration_apprentissage; i++) {
        RandomPick(database_learning_y, m, 0, &index1, &index2);

        x1 = database_learning_x[index1]; y1 = database_learning_y[index1];
        x2 = database_learning_x[index2]; y2 = database_learning_y[index2];


        char indifference_vrai = (fabs(y1 - y2) <= tolerance) ? 1 : 0;

        if(CalculPhiX(var_phi, x1, q_phi, n, k, method) == EXIT_FAILURE) {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x dans Algorithm_Learning_Imprecis.\n");
            exit(EXIT_FAILURE);
        }
        if(CalculPhiX(var_phi, x2, q_phi_prime, n, k, method) == EXIT_FAILURE) {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x_prime dans Algorithm_Learning_Imprecis.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < taille_phi; i++) {
            if (isnan(var_phi[i]) || isinf(var_phi[i])) {
                fprintf(stderr, "ERREUR: var_phi[%d] = %f (NaN/Inf)\n", i, var_phi[i]);
                exit(EXIT_FAILURE);
            }
        }

        

        y_inf       = ProduitScalaire(q_phi_inf, var_phi, taille_phi);
        y_sup       = ProduitScalaire(q_phi_sup, var_phi, taille_phi);
        y_prime_inf = ProduitScalaire(q_phi_prime_inf, var_phi, taille_phi);
        y_prime_sup = ProduitScalaire(q_phi_prime_sup, var_phi, taille_phi);


        // Appel de la bonne fonction de mise à jour selon use_L1 et ordre
        if(update_imprecis_func(
            var_phi,
            q_phi_inf, q_phi_sup, q_phi_prime_inf, q_phi_prime_sup,
            y_inf, y_sup, y_prime_inf, y_prime_sup,
            indifference_vrai, beta, gamma, n, epoch
            ) == EXIT_FAILURE) {
            printf("Erreur dans Algorithm_Learning_Imprecis");
            exit(EXIT_FAILURE);
        }

        

        // Mise à jour des compteurs de performance
        Compteurs_Imprecis(&resultats, indifference_vrai, ordre, y_inf, y_sup, y_prime_inf, y_prime_sup);
    }

    free(q_phi);
    free(q_phi_prime);

    double erreur_moy = Error_Mean_Imprecis(
        database_learning_x, database_learning_y, var_phi,
        tolerance, 1000, m, n, k, method, ordre, use_L1
    );
    resultats.erreur_moy = erreur_moy;

    if (1) {
        FILE *logf3 = fopen("problemes.csv", "a");
        if (!logf3) {
            perror("Impossible d'ouvrir problemes.csv");
            exit(EXIT_FAILURE);
        }

        fprintf(logf3, "%d,", epoch);
        fprintf(logf3, "training,");


        // Sauvegarde de x1
        for (int j = 0; j < n; j++) {
            fprintf(logf3, "%.6f", x1[j]);
            if (j < n - 1) fprintf(logf3, " ");
        }
        fprintf(logf3, ",");

        // Sauvegarde de x2
        for (int j = 0; j < n; j++) {
            fprintf(logf3, "%.6f", x2[j]);
            if (j < n - 1) fprintf(logf3, " ");
        }
        fprintf(logf3, ",");

        // Sauvegarde de var_phi
        for (int j = 0; j < taille_phi; j++) {
            fprintf(logf3, "%.6f", var_phi[j]);
            if (j < taille_phi - 1) fprintf(logf3, " ");
        }
        fprintf(logf3, ",");

        // Sauvegarde des y
        fprintf(logf3, "%.10f,%.10f,%.10f,%.10f,%f\n", y_inf, y_sup, y_prime_inf, y_prime_sup, erreur_moy);

        fclose(logf3);
    }



    if (epoch % 100 == 0) // Affiche toutes les 10 époques  
            printf("Époque %d : Nombre indifférences = %d ; Nombre préférences = %d Erreur moyenne = %.10f\n Indifférences vraies = %d, Indifférences fausses = %d, Préférences vraies = %d, Préférences fausses = %d, Préférences inversées = %d\n",
                    epoch, resultats.nb_indifference, resultats.nb_preference, resultats.erreur_moy, resultats.nb_indifference_vraie, resultats.nb_indifference_fausse, resultats.nb_preference_vraie, resultats.nb_preference_fausse, resultats.nb_preference_inverse);


    return resultats;
}



double Error_Mean_Imprecis(
    double **database_x, double *database_y, double *var_phi,
    double tolerance, int nb_samples, int m, int n, int k, int method, int ordre, int use_L1
) 
{
    double sum_error = 0.0;
    int index1, index2;
    double y_inf, y_sup, y_prime_inf, y_prime_sup;
    double y1, y2, *x1, *x2;

    double *q_phi       = (double *)malloc(taille_phi * 2 * sizeof(double));
    double *q_phi_prime = (double *)malloc(taille_phi * 2 * sizeof(double));
    if (q_phi == NULL || q_phi_prime == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour q_phi ou q_phi_prime. Localisation : Error_Mean_Imprecis\n");
        exit(EXIT_FAILURE);
    }
    double *q_phi_inf = q_phi,             *q_phi_sup = q_phi + taille_phi;
    double *q_phi_prime_inf = q_phi_prime, *q_phi_prime_sup = q_phi_prime + taille_phi; 

    // Définition du type de pointeur de fonction pour l'erreur imprécise
    typedef double (*ErrorImprecisFunc)(
        double y_inf, double y_sup, double y_prime_inf, double y_prime_sup, char indifference_vrai
    );

    ErrorImprecisFunc error_imprecis_func = NULL;
    if (use_L1) {
        if (ordre == FAIBLE)
            error_imprecis_func = L1_Error_Imprecis_FAIBLE;
        else
            error_imprecis_func = L1_Error_Imprecis_FORT;
    } else {
        if (ordre == FAIBLE)
            error_imprecis_func = L2_Error_Imprecis_FAIBLE;
        else
            error_imprecis_func = L2_Error_Imprecis_FORT;
    }


    for (int i = 0; i < nb_samples; i++) {
        RandomPick(database_y, m, 1, &index1, &index2);

        x1 = database_x[index1]; y1 = database_y[index1];
        x2 = database_x[index2]; y2 = database_y[index2];

        char indifference_vrai = (fabs(y1 - y2) <= tolerance) ? 1 : 0;

        if(CalculPhiX(var_phi, x1, q_phi, n, k, method) == EXIT_FAILURE) {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x dans Error_Mean_Imprecis.\n");
            fprintf(stderr, "i=%d, index1=%d, n=%d, k=%d, method=%d\n", i, index1, n, k, method);
            exit(EXIT_FAILURE);
        }
        if(CalculPhiX(var_phi, x2, q_phi_prime, n, k, method) == EXIT_FAILURE) {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x_prime dans Error_Mean_Imprecis.\n");
            fprintf(stderr, "i=%d, index2=%d, n=%d, k=%d, method=%d\n", i, index2, n, k, method);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < taille_phi; i++) {
            if (isnan(var_phi[i]) || isinf(var_phi[i])) {
                fprintf(stderr, "ERREUR: var_phi[%d] = %f (NaN/Inf)\n", i, var_phi[i]);
                exit(EXIT_FAILURE);
            }
        }

        y_inf = ProduitScalaire(q_phi_inf, var_phi, taille_phi);
        y_sup = ProduitScalaire(q_phi_sup, var_phi, taille_phi);
        y_prime_inf = ProduitScalaire(q_phi_prime_inf, var_phi, taille_phi);
        y_prime_sup = ProduitScalaire(q_phi_prime_sup, var_phi, taille_phi);

        double err = 0.0;
        err = error_imprecis_func(y_inf, y_sup, y_prime_inf, y_prime_sup, indifference_vrai);

        sum_error += err;
    }

    free(q_phi);
    free(q_phi_prime);

    double mean_error = sum_error / (double) nb_samples;
    if (isnan(mean_error) || isinf(mean_error)) {
        fprintf(stderr, "NaN/Inf detected in mean_error: sum_error=%f, nb_samples=%d\n", sum_error, nb_samples);
        exit(EXIT_FAILURE);
    }
    return mean_error;
}



ResultatsAlgo Algorithm_Test_Imprecis(double **database_test_x, double *database_test_y, double *var_phi, int n, int m, int k, int iteration_test, double err_precedente, double tolerance, double delta, int method, int use_L1, int ordre) 
{
    (void) delta;
    int index1, index2;
    double y_inf, y_sup, y_prime_inf, y_prime_sup;
    double y1, y2, *x1, *x2;
    ResultatsAlgo resultats = {0, 0, 0, 0, 0, 0, 0, 0};
    
    double *q_phi       = (double *)malloc(taille_phi * 2 * sizeof(double));
    double *q_phi_prime = (double *)malloc(taille_phi * 2 * sizeof(double));
    if (q_phi == NULL || q_phi_prime == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour q_phi ou q_phi_prime. Localisation : Algorithm_test_imprecis\n");
        exit(EXIT_FAILURE);
    }
    double *q_phi_inf = q_phi,             *q_phi_sup = q_phi + taille_phi;
    double *q_phi_prime_inf = q_phi_prime, *q_phi_prime_sup = q_phi_prime + taille_phi; 

    // Initialisation des compteurs
    char indifference_vrai ;

    for (int i = 0; i < iteration_test; i++) {
        RandomPick(database_test_y, m, 0, &index1, &index2);

        x1 = database_test_x[index1] ;
        y1 = database_test_y[index1] ;
        x2 = database_test_x[index2] ;
        y2 = database_test_y[index2] ;

        indifference_vrai = (fabs(y1 - y2) <= tolerance) ? 1 : 0 ;

        if(CalculPhiX(var_phi, x1, q_phi, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x dans Algorithm_test_imprecis.\n");
            exit(EXIT_FAILURE);
        }
        if(CalculPhiX(var_phi, x2, q_phi_prime, n, k, method) == EXIT_FAILURE)
        {
            fprintf(stderr, "Erreur lors de l'appel à CalculPhiX de x_prime dans Algorithm_test_imprecis.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < taille_phi; i++) {
            if (isnan(var_phi[i]) || isinf(var_phi[i])) {
                fprintf(stderr, "ERREUR: var_phi[%d] = %f (NaN/Inf)\n", i, var_phi[i]);
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < taille_phi; i++) {
            if (isnan(var_phi[i]) || isinf(var_phi[i])) {
                fprintf(stderr, "ERREUR: var_phi[%d] = %f (NaN/Inf)\n", i, var_phi[i]);
                exit(EXIT_FAILURE);
            }
        }

        y_inf = ProduitScalaire(q_phi_inf, var_phi, n);
        y_sup = ProduitScalaire(q_phi_sup, var_phi, n);
        y_prime_inf = ProduitScalaire(q_phi_prime_inf, var_phi, n);
        y_prime_sup = ProduitScalaire(q_phi_prime_sup, var_phi, n);



        // Mise à jour des compteurs de performance
        Compteurs_Imprecis(&resultats, indifference_vrai, ordre, y_inf, y_sup, y_prime_inf, y_prime_sup);
    }
    // Libération de la mémoire
    free(q_phi);
    free(q_phi_prime);

    double erreur_moy = Error_Mean_Imprecis(
        database_test_x, database_test_y, var_phi,
        tolerance, 1000, m, n, k, method, ordre, use_L1
    );
    resultats.erreur_moy = erreur_moy;

    return resultats;
}

















int main_algo(double **database_learning_x, double *database_learning_y, int m_learning, double **database_test_x, double *database_test_y, int m_test, int n, int k, int iteration, int iteration_apprentissage, int iteration_test, double beta, double gamma, double delta, double tolerance, int use_L1, int method, int ordre) {
    srand(time(NULL));

        // 0) Affichag des paramètres d'apprentissage
    printf("Paramètres d'apprentissage :\n");
    printf("  - Nombre total d'itérations de l'algorithme : %d\n", iteration);
    printf("  - Nombre d'itérations d'apprentissage       : %d\n", iteration_apprentissage);
    printf("  - Nombre d'itérations de test               : %d\n", iteration_test);

    // Méthode d'apprentissage
    printf("  - Méthode d'apprentissage : ");
    switch (method) {
        case LINEAR:
            printf("Linéaire\n");
            printf("    - Paramètre de régularisation L1 (gamma) : %.2e\n", gamma);
            printf("    - Utilisation de L1                      : %s\n", use_L1 ? "Oui" : "Non");
            printf("    - Paramètre de mise à jour (delta)       : %.2e\n", delta);
            break;

        case GAME:
            printf("GAME\n");
            if (k == -1)
                k = n;
            printf("    - Nombre de k-additive                   : %d\n", k);
            printf("    - Taille de var_phi                      : %lld\n", CalcuTailleGame(n, k));
            printf("    - Utilisation de L1                      : %s\n", use_L1 ? "Oui" : "Non");
            printf("    - Paramètre de mise à jour (delta)       : %.2e\n", delta);
            break;

        case MACSUM:
            printf("MacSum\n");
            printf("    - Utilisation de L1                      : %s\n", use_L1 ? "Oui" : "Non");
            printf("    - Paramètre de mise à jour (delta)       : %.2e\n", delta);
            break;

        case MACSUM_IMPRECIS:
            printf("MacSum Imprécis\n");
            printf("    - Delta non requis pour cette méthode.\n");
            printf("    - Ordre de l'algorithme : %s\n", ordre ? "FORT" : "FAIBLE");
            break;

        default:
            fprintf(stderr, "Erreur : méthode d'apprentissage non reconnue (localisation : Algorithm_learning)\n");
            return EXIT_FAILURE;
    }

    // Autres paramètres
    printf("  - Taux d'apprentissage (beta)              : %.2e\n", beta);
    printf("  - Tolérance                                : %.4f\n", tolerance);

    // Dimensions des jeux de données
    printf("  - Dimensions des tableaux :\n");
    printf("    - Apprentissage : %dx%d\n", m_learning, n);
    printf("    - Test          : %dx%d\n", m_test, n);
    printf("\n\n");



            // 1.0) Vérification des paramètres
    // Vérification de k (spécifique à GAME)
    if (method == GAME && (k <= 0 || k > n)) {
        fprintf(stderr, "Erreur : le paramètre k = %d doit être dans l'intervalle [1, %d]. (Localisation : main)\n", k, n);
        return EXIT_FAILURE;
    }

    
        // 1.1) Initialisation & Génération de var_phi selon le mode d'apprentissage
    if (method == LINEAR || method == MACSUM) {
        taille_phi = n;
    } else if (method == GAME) {
        taille_phi = CalcuTailleGame(n, k);
    }
    else if(method == MACSUM_IMPRECIS) {
        taille_phi = n;
    }

    double *var_phi = (double *)malloc(taille_phi * sizeof(double));
    if (var_phi == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour var_phi. Localisation : main_algo\n");
        exit(EXIT_FAILURE);
    }
    srand(0);  // 👈 graine fixe = comportement déterministe
    if (use_L1) {
        for (int i = 0; i < taille_phi; i++) {
            var_phi[i] = 0.0;
        }
    } else {
        double norm = 0.0;
        for (int i = 0; i < taille_phi; i++) {
            var_phi[i] = (double)rand() / RAND_MAX;
            norm += var_phi[i] * var_phi[i];
        }
        norm = sqrt(norm);
        if (norm > 0.0) {
            for (int i = 0; i < taille_phi; i++) {
                var_phi[i] /= norm;
            }
        }
    }
    srand(time(NULL));  // 👈 remet une graine variable basée sur l'horloge

        // 1.2) Initialiser un array pour memoriser var_phi & l'erreur
    double *var_phi_memory = (double*) malloc(taille_phi * sizeof(double));
    double err_memory;
    ResultatsAlgo learning, test;
    learning.erreur_moy = test.erreur_moy = INFINITY;

    memcpy(var_phi_memory, var_phi, taille_phi * sizeof(double));
    err_memory = INFINITY;


        // 2.1) Initialise les fichiers de log
    FILE *logf = fopen("training_log.csv", "w");
    if (!logf) {
        perror("Impossible d'ouvrir training_log.csv");
        exit(EXIT_FAILURE);
    }
    fprintf(logf, "iteration,erreur_moy,nombre_egalite,nombre_preference,pred_vrai_egalite,pred_fausse_egalite,pred_vrai_preference,pred_fausse_preference,pred_fausse_inversion\n");
    fclose(logf);

    FILE *logf2 = fopen("test_log.csv", "w");
    if (!logf2) {
        perror("Impossible d'ouvrir training_log2.csv");
        fclose(logf);
        exit(EXIT_FAILURE);
    }
    fprintf(logf2, "iteration,erreur_moy,nombre_egalite,nombre_preference,pred_vrai_egalite,pred_fausse_egalite,pred_vrai_preference,pred_fausse_preference,pred_fausse_inversion\n");
    fclose(logf2);
    FILE *logf3 = fopen("problemes.csv", "w");
    if (!logf3) {
        perror("Impossible d'ouvrir problemes.csv");
        fclose(logf); fclose(logf2);
        exit(EXIT_FAILURE);
    }
    fprintf(logf3, "iteration,boucle,x,x_prime,var_phi,y_inf,y_sup,y'_inf, y'_sup, erreur moyenne\n");
    fclose(logf3);



    // Créer le pointeur de fonction vers les algos généraux ou imprecis
    typedef ResultatsAlgo (*AlgoLearningFunc)(
        double **, double *, double *, int, int, int, int, int, double, double, double, double, double, int, int, int
    );
    typedef ResultatsAlgo (*AlgoTestFunc)(
        double **, double *, double *, int, int, int, int, double, double, double, int, int, int
    );


    AlgoLearningFunc algo_learning_func = NULL;
    AlgoTestFunc algo_test_func     = NULL;
    switch (method) {
        case LINEAR:
        case GAME:
        case MACSUM:
            algo_learning_func = (AlgoLearningFunc)Algorithm_learning;
            algo_test_func     = (AlgoTestFunc)Algorithm_test;
            break;
        case MACSUM_IMPRECIS:
            algo_learning_func = (AlgoLearningFunc)Algorithm_Learning_Imprecis;
            algo_test_func     = (AlgoTestFunc)Algorithm_Test_Imprecis;
            break;
        default:
            fprintf(stderr, "Méthode d'apprentissage non reconnue pour le pointeur de fonction.\n");
            exit(EXIT_FAILURE);
    }


    // int stuck = 0;

        // 3) Lancement de la boucle
    printf("::: Début de l'algorithme :::\n");
   
    for (int i = 0; i < iteration; i++) {
            // 3.1) Préparation des logs
        logf = fopen("training_log.csv", "a");
        logf2 = fopen("test_log.csv", "a");
        if (!logf || !logf2) {
            perror("Impossible d'ouvrir les fichiers de log");
            exit(EXIT_FAILURE);
        }

        
       
        if (algo_learning_func == NULL) {
            fprintf(stderr, "[CRITICAL] update_imprecis_func est NULL !\n");
            exit(EXIT_FAILURE);
        }
            // 3.2) Appel de l'algorithme d'apprentissage
        learning = algo_learning_func(
            database_learning_x, database_learning_y, var_phi, i+1, n, m_learning, k,
            iteration_apprentissage, learning.erreur_moy, beta, delta, gamma, tolerance, use_L1, method, ordre
        );

        
       

        // double **database_test_x, double *database_test_y, double *var_phi, int n, int m, int k, int iteration_test, double err_precedente, double tolerance, double delta, int method, int use_L1, int ordre
        test = algo_test_func(database_test_x, database_test_y, var_phi, n, m_test, k, iteration_test, err_memory, tolerance, delta, method, use_L1, ordre);

        memcpy(var_phi_memory, var_phi, taille_phi * sizeof(double));
        err_memory = test.erreur_moy;
        // stuck = 0;

        // Log des résultats d'apprentissage
        fprintf(logf, "%i,%f,%d,%d,%d,%d,%d,%d,%d\n",
            i,
            learning.erreur_moy,
            learning.nb_indifference,
            learning.nb_preference,
            learning.nb_indifference_vraie,
            learning.nb_indifference_fausse,
            learning.nb_preference_vraie,
            learning.nb_preference_fausse,
            learning.nb_preference_inverse
        );
        // Log des résultats de test
        fprintf(logf2, "%i,%f,%d,%d,%d,%d,%d,%d,%d\n",
            i,
            test.erreur_moy,
            test.nb_indifference,
            test.nb_preference,
            test.nb_indifference_vraie,
            test.nb_indifference_fausse,
            test.nb_preference_vraie,
            test.nb_preference_fausse,
            test.nb_preference_inverse
        );


        fclose(logf);
        fclose(logf2);
    }

        // 3.1) Affichage des résultats
    printf("Paramètre après apprentissage : \n");
    printf("  - Beta : %f\n", beta);
    printf("  - var_phi = [");
    for (int i = 0; i < taille_phi; i++) {
        printf("%.6f", var_phi[i]);
        if (i < taille_phi - 1) printf(", ");
    }
    printf("]\n");



        // 3.2) Libération des ressources
    free(var_phi);
    free(var_phi_memory);

    return EXIT_SUCCESS;
}


