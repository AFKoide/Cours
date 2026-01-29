#include "algorithm.h"

void Compteurs(ResultatsAlgo *compteurs, PreferenceType preference, double y, double y_prime, double Delta)
{
    char indifference_predite = 0, preference_predite = 0, preference_inverse = 0;

    // printf("Prédiction Calcul : y=%.5f, y'=%.5f, delta = %.5f\n", y, y_prime, Delta);


    // Utilisation directe de y et y_prime

    preference_predite   = (y > (y_prime + Delta)) ? 1 : 0;
    indifference_predite = (fabs(y - y_prime) < Delta) ? 1 : 0;
    preference_inverse   = ((y + Delta) < y_prime) ? 1 : 0;

    if (preference == INDIFFERENCE) {
        compteurs->nb_indifference++;
        if (indifference_predite)
            compteurs->nb_indifference_vraie++;
        else
            compteurs->nb_indifference_fausse++;
    } 
    else if (preference == STRICT_PREFERENCE) {
        compteurs->nb_preference++;
        if (preference_predite)
            compteurs->nb_preference_vraie++;
        else if (preference_inverse)
            compteurs->nb_preference_inverse++;
        else 
            compteurs->nb_preference_fausse++;
    }
}





// --------------------------------------------------------------------------------
// Learning_algorithm : Algorithme utilisé pour mettre à jour les poids.
// --------------------------------------------------------------------------------
ResultatsAlgo Learning_Algorithm(
    double **database_x, double *database_y, int n, int m, int k,
    double *var_phi, int epoch, int iteration_apprentissage, 
    double lambda, double delta, double gamma, double tolerance, double rho) 
{
    int index1, index2;
    double y, y_prime;
    double *x, *x_prime;
    ResultatsAlgo resultats = {0, 0, 0, 0, 0, 0, 0, 0};

    PreferenceType preference;

    // Initialisation de B et BT
    int indice = subset_sums(n, k, &B);
    if(indice == 0) {
        fprintf(stderr, "[ERREUR] subset_sums a retourné un NAN\n");
        BUFFER; exit(EXIT_FAILURE);
    }

    if (!B || indice <= 0) {
        fprintf(stderr, "[ERREUR] B non initialisée ou indice <= 0 (indice = %d)\n", indice);
        BUFFER; exit(EXIT_FAILURE);
    }


    if (TransposeMatrix(B, indice, n, &BT) != EXIT_SUCCESS) {
        fprintf(stderr, "[ERREUR] TransposeMatrix a échoué\n");
        BUFFER; exit(EXIT_FAILURE);
    }


    if (Init_ADMMRDA(n, indice) != EXIT_SUCCESS) {
        fprintf(stderr, "[ERREUR] Init_ADMMRDA a échoué\n");
        BUFFER; exit(EXIT_FAILURE);
    }



    for (int i = 0; i < iteration_apprentissage; i++) {
        RandomPick(database_y, m, 0, tolerance, &index1, &index2);

        // Charger depuis les db les lignes necessaires
        x = database_x[index1] ;
        y = database_y[index1] ;
        x_prime = database_x[index2] ;
        y_prime = database_y[index2] ;

        if(fabs(y - y_prime) - tolerance > 0.0)
            preference = STRICT_PREFERENCE;
        else
            preference = INDIFFERENCE;

        // printf("Tableau de données : y = %.5f ; y' = %.5f ; Tolérance = %.5f ; Préférence = %s          -          ", y, y_prime, tolerance, preference ? "Indifference" : "Stricte Préference");



        // Calculer y avec var_phi et q_phi
        y       = ProduitScalaire(x, var_phi, n) ;
        y_prime = ProduitScalaire(x_prime, var_phi, n) ;

        // Mise à jour de var_phi
        if(Learning_ADMMRDA(preference, var_phi, x, x_prime, y, y_prime, indice, i+1, gamma, lambda, delta, rho, n, k) != EXIT_SUCCESS) {
            fprintf(stderr, "Erreur dans la fonction d'apprentissage ADMM-RDA"); BUFFER;    
            BUFFER; exit(EXIT_FAILURE);
        }
        
        // Compte les prédictions
        Compteurs(&resultats, preference, y, y_prime, delta);
    }

    // Libérer la mémoire
    Destroy_ADMMRDA(n, indice);

    // Retour les compteurs
    return resultats;
}


// --------------------------------------------------------------------------------
// Test_algorithm : Algorithme utilisé pour tester l'apprentissage.
// --------------------------------------------------------------------------------
double Error_mean(double **database_x, double *database_y, double *var_phi, double tolerance, double delta, int m, int n, int k) 
{   
    int nb_samples = 1000;
    double sum_error = 0.0;
    int index1, index2;
    double y, y_prime;
    double *x, *x_prime;

    PreferenceType preference;

    for (int i = 0; i < nb_samples; i++) {
        RandomPick(database_y, m, 0, tolerance, &index1, &index2);

        x       = database_x[index1] ;
        y       = database_y[index1] ;
        x_prime = database_x[index2] ;
        y_prime = database_y[index2] ;

        if((y - y_prime) > tolerance)
            preference = STRICT_PREFERENCE;
        else
            preference = INDIFFERENCE;

        y       = ProduitScalaire(x,var_phi,n) ;
        y_prime = ProduitScalaire(x_prime,var_phi,n) ;
        
        sum_error += Error_ADMMRDA(preference, y, y_prime, delta);
    }

    if(isnan(sum_error)) {
        fprintf(stderr, "[Erreur] - Error_mean contient un NaN.");
        BUFFER; exit(EXIT_FAILURE);
    }

    return sum_error / (double) nb_samples;
}


ResultatsAlgo Test_Algorithm(double **database_x, double *database_y, double *var_phi, int n, int m, int k, int iteration_test, double tolerance, double delta) 
{
    int index1, index2;
    double y, y_prime;
    double *x, *x_prime;
    ResultatsAlgo resultats = {0, 0, 0, 0, 0, 0, 0, 0};
    PreferenceType preference;

    for (int i = 0; i < iteration_test; i++) {
        RandomPick(database_y, m, 0, tolerance, &index1, &index2);

        // Charger depuis les db les lignes necessaires
        x       = database_x[index1] ;
        y       = database_y[index1] ;
        x_prime = database_x[index2] ;
        y_prime = database_y[index2] ;

        if(fabs(y - y_prime) - tolerance > 0.0)
            preference = STRICT_PREFERENCE;
        else
            preference = INDIFFERENCE;
        // printf("Tableau de données : y = %.5f ; y' = %.5f ; Tolérance = %.5f ; Préférence = %s          -          ", y, y_prime, tolerance, preference ? "Indifference" : "Stricte Préference");


        // Calculer y avec var_phi et x
        y       = ProduitScalaire(x, var_phi, n) ;
        y_prime = ProduitScalaire(x_prime, var_phi, n) ;

        // Compte les prédictions
        Compteurs(&resultats, preference, y, y_prime, delta);
    }

    double erreur_moy = Error_mean(database_x, database_y, var_phi, tolerance, delta, m, n, k);
    resultats.erreur_moy = erreur_moy;

    return resultats;
}












int main_algo(double **database_learning_x, double *database_learning_y, int m_learning, double **database_test_x, double *database_test_y, int m_test, int n, int k, 
    int iteration, int iteration_apprentissage, int iteration_test, 
    double lambda, double gamma, double delta, double tolerance, double rho) 
{
    srand(time(NULL));
            // -- 1.0) Initialisation
        // 1.1) Vérification de k
    if(k == -1)
        k = n;
    else if (k > n && k < -1) {
        fprintf(stderr, "ERREUR : k doit être compris entre 0 et %d", n);
        BUFFER; exit(EXIT_FAILURE);
    }

        // 1.2) Initilisation du vecteur de poids
    double *var_phi = (double *)calloc(n, sizeof(double));
    if (var_phi == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour m. Localisation : main_algo\n");
        BUFFER; exit(EXIT_FAILURE);
    }
    // for (int i = 0; i < n; i++)
    // {
    //     var_phi[i] = gaussianNorm();
    //     printf("%.17f\n", var_phi[i]);
    // }
    

        // 1.3) Initialiser un array pour memoriser var_phi & l'erreur
    ResultatsAlgo learning, test;

        // 1.4) Initialise les fichiers de log
    FILE *logf = fopen("training_log.csv", "w");
    if (!logf) {
        fprintf(stderr, "Impossible d'ouvrir training_log.csv");
        BUFFER; exit(EXIT_FAILURE);
    }
    fprintf(logf, "iteration,erreur_moy,nombre_egalite,nombre_preference,pred_vrai_egalite,pred_fausse_egalite,pred_vrai_preference,pred_fausse_preference,pred_fausse_inversion\n");
    fclose(logf);

    FILE *logf2 = fopen("test_log.csv", "w");
    if (!logf2) {
        fprintf(stderr, "Impossible d'ouvrir training_log2.csv");
        fclose(logf);
        BUFFER; exit(EXIT_FAILURE);
    }
    fprintf(logf2, "iteration,erreur_moy,nombre_egalite,nombre_preference,pred_vrai_egalite,pred_fausse_egalite,pred_vrai_preference,pred_fausse_preference,pred_fausse_inversion\n");
    fclose(logf2);


            // -- 2) Lancement de la boucle
    printf("::: Début de l'algorithme :::\n");
    
    for (int i = 0; i < iteration; i++) {
        // printf("Itération n°%d\n", i);
            // 2.1) Préparation des logs
        logf = fopen("training_log.csv", "a");
        logf2 = fopen("test_log.csv", "a");
        if (!logf || !logf2) {
            fprintf(stderr, "Impossible d'ouvrir les fichiers de log");
            BUFFER; exit(EXIT_FAILURE);
        }

            // 2.2) Appel de l'algorithme d'apprentissage
        learning = Learning_Algorithm(database_learning_x, database_learning_y, n, m_learning, k, var_phi, i+1, iteration_apprentissage, lambda, delta, gamma, tolerance, rho);

            // 2.3) Appel de l'algorithme de test
        test = Test_Algorithm(database_test_x, database_test_y, var_phi, n, m_test, k, iteration_test, tolerance, delta);



        if(i % 1000 == 0) {
            printf("::: Debug iteration n°%d :::\n", i);
            printf("erreur moyenne : %f\n", test.erreur_moy),
            printf("nb_indifference_vraie : %d, nb_indifference_fausse : %d,\n nb_preference_vraie : %d, nb_preference_fausse : %d, nb_preference_inverse : %d\n", test.nb_indifference_vraie, test.nb_indifference_fausse, test.nb_preference_vraie, test.nb_preference_fausse, test.nb_preference_inverse);
        }


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


        // 3.2) Libération des ressources
    free(var_phi);

    return EXIT_SUCCESS;
}


