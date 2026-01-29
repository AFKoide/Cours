#include "csv_utils.h"



char *trim_whitespace(char *str) {
    // Enlever les espaces en début
    while (isspace((unsigned char)*str)) 
        str++;

    if (*str == 0) return str; // chaîne vide après trim

    // Enlever les espaces en fin
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Ajouter fin de chaîne
    end[1] = '\0';

    return str;
}


int charger_melanger_csv(const char *path, int col_cible, /*output*/ double ***X_total, double **y_total, int *m_total, int *n) 
{
    // Ouverture du fichier CSV
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier CSV");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE];
    int num_cols = 0;
    int num_rows = 0;


    // Compter le nombre de colonnes. On ignore l'entête.
    if (!fgets(line, MAX_LINE, f)) {
        fprintf(stderr, "Fichier vide\n");
        fclose(f);
        return EXIT_FAILURE;
    }
    if (fgets(line, MAX_LINE, f)) {
        char *token = strtok(line, ",;\n");
        while (token) {
            ++num_cols;
            token = strtok(NULL, ",;\n");
        }
    } else {
        fprintf(stderr, "Fichier vide\n");
        fclose(f);
        return EXIT_FAILURE;
    }


    if(col_cible == -1) {
        printf("`col_cible` = %d, Selection de la dernière colonne comme cible\n", col_cible);
        col_cible = num_cols - 1; // Par défaut, la dernière colonne est la cible
    }

    if (col_cible < 0 || col_cible >= num_cols) {
        fprintf(stderr, "`col_cible` = %d, Indice de colonne cible invalide\n", col_cible);
        fclose(f);
        return EXIT_FAILURE;
    }


    // Compter les lignes (données)
    while (fgets(line, MAX_LINE, f)) ++num_rows;
    rewind(f);
    if (!fgets(line, MAX_LINE, f)) {
        fprintf(stderr, "Erreur lors de la lecture de l'en-tête CSV\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    *n = num_cols - 1;      // Nombre de caractéristiques (toutes sauf la cible)
    *m_total = num_rows;    // Nombre total de lignes

    printf("Nombre de lignes : %d, Nombre de colonnes : %d\n", num_rows, num_cols);

    // Création du tableau d'indices & shuffle
    srand(time(NULL));
    int *indices = malloc(num_rows * sizeof(int));
    for (int i = 0; i < num_rows; ++i) {
        indices[i] = i;
    }
    for (int i = num_rows - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
    }


    *X_total = (double **)malloc(num_rows * sizeof(double *));
    if(!X_total) {
        printf("Erreur allocation y_total\n");
        free(indices);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    *y_total = (double *)malloc(num_rows * sizeof(double));
    if(!y_total) {
        printf("Erreur allocation y_total\n");
        free(indices); free(X_total);
        fclose(f);
        exit(EXIT_FAILURE);
    }

    for( int i = 0 ; i< num_rows ; i++ )
    {
        (*X_total)[i] = (double *)malloc((*n) * sizeof(double));
        if (!(*X_total)[i]) {
            printf("Erreur allocation X_total[%d]\n", i);
            // Libérer la mémoire déjà allouée
            for (int j = 0; j < i; ++j) free((*X_total)[j]);
            free(*X_total);
            free(*y_total);
            free(indices);
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }


    // Lecture avec injection directe dans l’ordre mélangé
    int current_row = 0;
    while (fgets(line, MAX_LINE, f) && current_row < num_rows) {
        int idx_shuffle = indices[current_row];
        // int idx_shuffle = current_row;

        char *saveptr = NULL;
        char *token = strtok_r(line, ",;\n", &saveptr);
        int col_idx = 0, x_idx = 0;

        while (token && col_idx < num_cols) {
            token = trim_whitespace(token);
            if (col_idx == col_cible) {
                (*y_total)[idx_shuffle] = atof(token);
            } else {
                (*X_total)[idx_shuffle][x_idx++] = atof(token);
            }
            token = strtok_r(NULL, ",;\n", &saveptr);
            col_idx++;
        }

        current_row++;
    }

    printf("Nombre de colonnes détecté : %d\n", num_cols);
    printf("Numéro de la colonne résultat (cible) : %d\n", col_cible);
    printf("Nombre de caractéristiques (features) : %d\n", *n);
    printf("Nombre total d'exemples (lignes de données) : %d\n", *m_total);
    printf("Première valeur cible (y_total[0]) : %.3f\n", (*y_total)[0]);
    printf("Première ligne de X_total : ");
    for (int i = 0; i < *n; ++i) {
        printf("%.3f ", (*X_total)[0][i]);
    }
    printf("\n");


    // Nettoyage
    free(indices);
    fclose(f);
    return EXIT_SUCCESS;
}


int separer_dataset(double **X_total, double *y_total, int m_total, int m_train, int m_test, /*output*/ double ***X_train, double **y_train, double ***X_test, double **y_test) 
{
    if (m_train + m_test > m_total) {
        fprintf(stderr, "Erreur : m_learning + m_test > m_total\n");
        return EXIT_FAILURE;
    }

    // Découpage des pointeurs sans recopier les données
    *X_train = X_total;
    *y_train = y_total;

    *X_test = X_total + m_train;
    *y_test = y_total + m_train;    

    return EXIT_SUCCESS;
}


// Fonction utilitaire pour libérer un dataset
void liberer_dataset(double **X, double *y, int m) 
{
    if (X) {
        for (int i = 0; i < m; ++i)
            free(X[i]);
        free(X);
    }
    free(y);
}