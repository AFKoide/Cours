#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "csv_utils.h"          // A inclure pour la lecture CSV
#include "algorithm.h"          // Contient les fonctions d'apprentissage et de test


#define LINEAR  0
#define GAME    1
#define MACSUM  2

#define MAX_LINE_LENGTH 4096

typedef struct {
    char csv_path[256];
    int col_cible;
    double target_pct;
    double pourcentage;
    int m_train;
    int m_test;
    int methode;
    int use_L1;
    int k;
    double beta;
    double delta;
    double gamma;
    int iteration;
    int iteration_apprentissage;
    int iteration_test;
    int ordre;
} Parametres;




void FakeData(double **X_total, double *y_total, int n, int m_total)
{
    double *phi ;
    int i ;
    phi = (double *)malloc(n*sizeof(double)) ;
    for( i=0 ; i<n ; i++) 
        phi[i] = gaussianRandom() / (double)n ;
    for(i=0 ; i<m_total ; i++) 
        y_total[i] = ProduitScalaire(phi, X_total[i], n)  ;
    
    free(phi) ;
    return ;
}



int compare_double(const double *a, const double *b) {
    return (*a > *b) - (*a < *b);
}

// Calcule un seuil tel que "pourcentage" des différences absolues y[i]-y[j] soient <= seuil
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



void normaliser_colonnes(double **X, double *y, int n, int m) {
    // Normalisation de X (colonnes : features)
    for (int j = 0; j < n; ++j) {
        double mean = 0.0, std = 0.0;

        for (int i = 0; i < m; ++i)
            mean += X[i][j];
        mean /= m;

        for (int i = 0; i < m; ++i)
            std += (X[i][j] - mean) * (X[i][j] - mean);
        std = sqrt(std / m);
        if (std == 0.0)
            std = 1.0;

        for (int i = 0; i < m; ++i)
            X[i][j] = (X[i][j] - mean) / std;
    }

    // Normalisation de y
    double mean_y = 0.0, std_y = 0.0;

    for (int i = 0; i < m; ++i)
        mean_y += y[i];
    mean_y /= m;

    for (int i = 0; i < m; ++i)
        std_y += (y[i] - mean_y) * (y[i] - mean_y);
    std_y = sqrt(std_y / m);
    if (std_y == 0.0)
        std_y = 1.0;

    for (int i = 0; i < m; ++i)
        y[i] = (y[i] - mean_y) / std_y;
}
// */

int charger_parametres(const char *fichier, Parametres *params) {
    FILE *fp = fopen(fichier, "r");
    if (!fp) {
        perror("Erreur ouverture fichier paramètres");
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
            case 1: params->col_cible = atoi(ligne); break;
            case 2: params->target_pct = atof(ligne); break;
            case 3: params->pourcentage = atof(ligne); break;
            case 4: params->m_train = atoi(ligne); break;
            case 5: params->m_test = atoi(ligne); break;
            case 6: params->methode = atoi(ligne); break;
            case 7: params->use_L1 = atoi(ligne); break;
            case 8: params->k = atoi(ligne); break;
            case 9: params->ordre = atoi(ligne); break;
            case 10: params->beta = atof(ligne); break;
            case 11: params->delta = atof(ligne); break;
            case 12: params->gamma = atof(ligne); break;
            case 13: params->iteration = atoi(ligne); break;
            case 14: params->iteration_apprentissage = atoi(ligne); break;
            case 15: params->iteration_test = atoi(ligne); break;
            default:
            fprintf(stderr, "Trop de lignes dans le fichier de paramètres.\n");
            fclose(fp);
            return 0;
        }

        ligne_num++;
    }

    fclose(fp);

    return 1;
}




int main(void) 
{
        // === Paramètres ===
    Parametres params;
    if (!charger_parametres("params.txt", &params)) {
        fprintf(stderr, "Erreur de chargement des paramètres.\n");
        return EXIT_FAILURE;
    }

    printf("=== Début de l'exécution ===\n");



        // === Chargement des données ===
    double **X_learning = NULL, *y_learning = NULL;
    double **X_test = NULL, *y_test = NULL;
    double **X_total = NULL, *y_total = NULL;
    int m_total = 0, m_train = 0, m_test = 0, n = 0; 
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



        // === Normalisation des données ===
    normaliser_colonnes(X_total, y_total, n, m_total);
    printf("Premières lignes de la base de données normalisée :\n");
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
    m_train = params.m_train;
    m_test = params.m_test;

    if (pourcentage > 0.0 && pourcentage < 100.0) {
        m_train = (int)((pourcentage / 100.0) * m_total);
        m_test = m_total - m_train;
    } else if ((pourcentage <= 0.0 || pourcentage >= 100.0) && (m_train <= 0 || m_test <= 0)) {
        fprintf(stderr, "Erreur : pourcentage doit être dans l'intervalle ]0,100[ si m_train et m_test sont nuls.\n");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    } else if (m_train <= 0 || m_test <= 0) {
        fprintf(stderr, "Erreur : m_train et m_test doivent être positifs.\n");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    } else if ((m_train + m_test) > m_total) {
        fprintf(stderr, "Erreur : la somme des deux populations doit être égal ou inférieur à la population totale de la db.");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    }


    if (separer_dataset(X_total, y_total, m_total, m_train, m_test,
                        &X_learning, &y_learning, &X_test, &y_test) 
                                                                    != EXIT_SUCCESS) {
        fprintf(stderr, "Erreur lors de la séparation du dataset.\n");
        liberer_dataset(X_total, y_total, m_total);
        return EXIT_FAILURE;
    }



    // === Lancement de l'apprentissage ===
    int result = main_algo(X_learning, y_learning, m_train,
                           X_test, y_test, m_test,
                           n, params.k, params.iteration,
                           params.iteration_apprentissage, params.iteration_test,
                           params.beta, params.gamma, params.delta, seuil, params.use_L1, params.methode, params.ordre);

    if (result != EXIT_SUCCESS)
        fprintf(stderr, "Erreur lors de l'exécution de l'algorithme.\n");



    // === Nettoyage ===
    liberer_dataset(X_total, y_total, m_total);

    printf("=== Fin de l'exécution ===\n");
    return result;
}
