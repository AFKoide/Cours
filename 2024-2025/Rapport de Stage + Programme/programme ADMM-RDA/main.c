// ===================
// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "csv_utils.h"          // A inclure pour la lecture CSV
#include "algorithm.h"          // Contient les fonctions d'apprentissage et de test
#include "functions.h"          // Trousse à outils


// ===================
// Define & Structures
#define MAX_LINE_LENGTH 4096

typedef struct {
    char csv_path[256];
    int iteration;
    int iteration_apprentissage;
    int iteration_test;
    int col_cible;
    double target_pct;
    double pourcentage;
    int m_train;
    int m_test;
    int k;
    double beta;                    // AKA Lambda
    double delta;
    double gamma;
    double rho;
} Parametres;



// ===================
// Fonction outils
int compare_double(const double *a, const double *b) {
    return (*a > *b) - (*a < *b);
}


double calculer_seuil(double *y, int taille, double pourcentage, int nb_paires)
{
    int i1, i2;
    double *diffs = malloc(nb_paires * sizeof(double));
    if (!diffs) {
        fprintf(stderr, "Erreur d'allocation pour diffs\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nb_paires; ++i) {
        i1 = rand() % taille;
        do {
            i2 = rand() % taille;
        } while (i1 == i2);

        diffs[i] = fabs(y[i1] - y[i2]);
    }

    qsort(diffs, nb_paires, sizeof(double), (int (*)(const void *, const void *))compare_double);

    int index = (int)(pourcentage * nb_paires);
    double seuil = diffs[index];
    
    printf("Seuil T = %.6f (tel que %.0f%% des delta sont ≤ T)\n", seuil, pourcentage * 100);

    int count_le = 0;
    for (int i = 0; i < nb_paires; ++i) {
        if (diffs[i] <= seuil) count_le++;
    }
    printf("Nombre de paires ≤ T : %d\n", count_le);
    printf("Nombre de paires >  T : %d\n", nb_paires - count_le);

    free(diffs);
    return seuil;
}


// ===================
// Fonction pour charger les paramètres du fichier texte
int charger_parametres(const char *fichier, Parametres *params) {
    FILE *fp = fopen(fichier, "r");
    if (!fp) {
        fprintf(stderr, "Erreur ouverture fichier paramètres");
        return 0;
    }

    char ligne[512];
    int ligne_num = 0;

    while (fgets(ligne, sizeof(ligne), fp)) {
        // Ignore les lignes vides ou commentaires
        if (ligne[0] == '%' || ligne[0] == '\n') continue;

        // Enlève le retour à la ligne
        ligne[strcspn(ligne, "\r\n")] = 0;

        switch (ligne_num) {
            case 0: 
                strncpy(params->csv_path, ligne, sizeof(params->csv_path));
                params->csv_path[sizeof(params->csv_path) - 1] = '\0'; // <- sécurité
                break;
            case 1: params->iteration               = atoi(ligne); break;
            case 2: params->iteration_apprentissage = atoi(ligne); break;
            case 3: params->iteration_test          = atoi(ligne); break;
            case 4: params->col_cible               = atoi(ligne); break;
            case 5: params->target_pct              = atof(ligne); break;
            case 6: params->pourcentage             = atof(ligne); break;
            case 7: params->m_train                 = atoi(ligne); break;
            case 8: params->m_test                  = atoi(ligne); break;
            case 9: params->k                       = atoi(ligne); break;
            case 10: params->beta                   = atof(ligne); break;
            case 11: params->delta                  = atof(ligne); break;
            case 12: params->gamma                  = atof(ligne); break;
            case 13: params->rho                    = atof(ligne); break;
            
            default:
                fprintf(stderr, "Trop de lignes dans le fichier de paramètres.\n");
                fclose(fp);
                return EXIT_FAILURE;
        }

        ligne_num++;
    }

    fclose(fp);

    return EXIT_SUCCESS;
}






// ============================================================================
int main(void) 
{
        // === Paramètres ===
    Parametres params;
    if (charger_parametres("params.txt", &params) != EXIT_SUCCESS) {
        fprintf(stderr, "Erreur de chargement des paramètres.\n");
        return EXIT_FAILURE;
    }

    printf("=== Début de l'exécution ===\n");



    
        // === Chargement des données ===
    double **X_learning = NULL, *y_learning = NULL;
    double **X_test = NULL, *y_test = NULL;
    double **X_total = NULL, *y_total = NULL;
    int m_total = 0, m_learning = 0, m_test = 0, n = 0; 
    srand((unsigned int)time(NULL));

    if (charger_melanger_csv(params.csv_path, params.col_cible, &X_total, &y_total, &m_total, &n) != EXIT_SUCCESS) {
        fprintf(stderr, "Erreur lors du chargement du CSV.\n");
        return EXIT_FAILURE;
    }


    // Affichage de quelques lignes du dataset original (avant normalisation)
    printf("Premières lignes de la base de données (avant normalisation) :\n");
    int lignes_afficher = m_total < 5 ? m_total : 5;
    for (int i = 0; i < lignes_afficher; ++i) {
        printf("X[%d]: [", i);
        for (int j = 0; j < n; ++j) {
            printf("%.4f", X_total[i][j]);
            if (j < n - 1) printf(", ");
        }
        printf("]  y: %.4f\n", y_total[i]);
    }



        // === Normalisation RDA ===
    double **X_normalise = NULL;
    int taille_n = 0;
    printf("\nn = %d\n", n);
    
    if (Normalisation_RDA(X_total, y_total, m_total, n, &X_normalise, &taille_n) != EXIT_SUCCESS) {
        fprintf(stderr, "Erreur lors de la normalisation RDA.\n");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    }

    // On libère X_total car remplacé
    for (int i = 0; i < m_total; ++i) {
        free(X_total[i]);
    }
    free(X_total);

    // Rediriger X_total vers les données normalisées
    X_total = X_normalise;
    n = taille_n;


    printf("Premières lignes de la base de données normalisée (RDA) :\n");
    int lignes_a_afficher = m_total < 5 ? m_total : 5;
    for (int i = 0; i < lignes_a_afficher; ++i) {
        printf("X[%d]: [", i);
        for (int j = 0; j < n; ++j) {
            printf("%.4f", X_total[i][j]);
            if (j < n - 1) printf(", ");
        }
        printf("]  y: %.4f\n", y_total[i]);
    }




        // === Calcul du seuil ===
    if(params.target_pct < 1 || params.target_pct > 100) {
        fprintf(stderr, "Erreur : le pourcentage d'égalité doit être compris entre ]0, 100[");
        exit(EXIT_FAILURE);
    }
    double seuil = calculer_seuil(y_total, m_total, params.target_pct / 100, m_total);




    // === Séparation des données en ensembles d'apprentissage et de test ===
    float pourcentage = params.pourcentage;
    m_learning = params.m_train;
    m_test = params.m_test;

    if (pourcentage > 0.0 && pourcentage < 100.0) {
        m_learning = (int)((pourcentage / 100.0) * m_total);
        m_test = m_total - m_learning;
    } else if ((pourcentage <= 0.0 || pourcentage >= 100.0) && (m_learning <= 0 || m_test <= 0)) {
        fprintf(stderr, "Erreur : pourcentage doit être dans l'intervalle ]0,100[ si m_learning et m_test sont nuls.\n");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    } else if (m_learning <= 0 || m_test <= 0) {
        fprintf(stderr, "Erreur : m_train et m_test doivent être positifs.\n");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    } else if ((m_learning + m_test) > m_total) {
        fprintf(stderr, "Erreur : la somme des deux populations doit être égal ou inférieur à la population totale de la db.");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    }


    if (separer_dataset(X_total, y_total, m_total, m_learning, m_test,
                        &X_learning, &y_learning, &X_test, &y_test) 
                                                                    != EXIT_SUCCESS) {
        fprintf(stderr, "Erreur lors de la séparation du dataset.\n");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    }



    // === Lancement de l'apprentissage ===
/* int main_algo(double **database_learning_x, double *database_learning_y, int m_learning, double **database_test_x, double *database_test_y, int m_test, int n, int k, 
 *    int iteration, int iteration_apprentissage, int iteration_test, 
 *    double lambda, double gamma, double delta, double tolerance, double rho) 
 */

    printf("\nn = %d\n", n);
    int result = main_algo(X_learning, y_learning, m_learning, X_test, y_test, m_test, n, params.k, 
                            params.iteration, params.iteration_apprentissage, params.iteration_test,
                            params.beta, params.gamma, params.delta, seuil, params.rho);

    if (result != EXIT_SUCCESS)
        fprintf(stderr, "Erreur lors de l'exécution de l'algorithme.\n");



    // === Nettoyage ===
    liberer_dataset(X_total, y_total, m_total);

    printf("=== Fin de l'exécution ===\n");
    return result;
}
