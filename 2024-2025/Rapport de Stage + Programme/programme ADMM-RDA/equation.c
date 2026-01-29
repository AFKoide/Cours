#include "equation.h"


// ===================
// Function to generate a matrix B of combinations of criteria
int subset_sums(int n, int k, double ***B_out) 
{
    int total_subsets = (1 << n);  // 2^n
    int count, indice = 0;

    // === Étape 1 : Compter combien de sous-ensembles valides il y a réellement
    int needed_subsets = 0;
    for (int i = 1; i < total_subsets - 1; i++) {  // Ignore empty and full set
        count = __builtin_popcount(i);
        if (count <= k) needed_subsets++;
    }

    // === Étape 2 : Allocation mémoire
    if (*B_out != NULL) {
        for (int i = 0; i < needed_subsets; i++) {
            if ((*B_out)[i]) free((*B_out)[i]);
        }
        free(*B_out);
        *B_out = NULL;
    }

    double **B = (double **)malloc(needed_subsets * sizeof(double *));
    if (!B) {
        fprintf(stderr, "Erreur d'allocation mémoire pour B\n");
        return 0;
    }

    // === Étape 3 : Remplir la matrice
    for (int i = 1; i < total_subsets - 1; i++) {
        count = __builtin_popcount(i);
        if (count > k) continue;

        B[indice] = (double *)calloc(n, sizeof(double));
        if (!B[indice]) {
            fprintf(stderr, "Erreur d'allocation pour B[%d]\n", indice);
            // Libérer tout ce qui a été déjà alloué
            for (int j = 0; j < indice; j++) free(B[j]);
            free(B);
            return 0;
        }

        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) B[indice][j] = -1.0;
        }

        indice++;
    }

    // === Étape 4 : Vérification finale et assignation
    if (indice != needed_subsets) {
        fprintf(stderr, "[ERREUR] Incohérence dans le nombre de sous-ensembles générés (attendu : %d, obtenu : %d)\n", needed_subsets, indice);
        for (int i = 0; i < indice; i++) free(B[i]);
        free(B);
        return 0;
    }

    *B_out = B;

    // printf("subset_sums -> n = %d ; k = %d ; total_subsets = %d ; needed_subsets = %d ; indice = %d\n",
    //        n, k, total_subsets, needed_subsets, indice);

    return indice;
}





// ==================================================================
double **B  = NULL;
double **BT = NULL;

// Variables globales pour garder en mémoire mu, z, g
double *mu = NULL;
double *z = NULL;
double *g_avg = NULL;


int Init_ADMMRDA(int n, int indice) 
{
    if (n <= 0) {
        fprintf(stderr, "Erreur: taille n invalide (%d) pour Γ_past\n", n);
        return EXIT_FAILURE;
    }

    // Création de g_past si n'existe pas, puis mise à jour de Γ_past
    if (g_avg == NULL) {
        g_avg = calloc(n, sizeof(double));
        if (g_avg == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour g_avg\n");
            return EXIT_FAILURE;
        }
    }
    else {
        fprintf(stderr, "Erreur : g_avg n'est pas NULL\n");
        return EXIT_FAILURE;
    }
    // Création de g_past si n'existe pas, puis mise à jour de Γ_past
    if (mu == NULL) {
        mu = calloc(indice, sizeof(double));
        if (mu == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour mu\n");
            return EXIT_FAILURE;
        }
    }
    else {
        fprintf(stderr, "Erreur : mu n'est pas NULL\n");
        return EXIT_FAILURE;
    }
    // Création de g_past si n'existe pas, puis mise à jour de Γ_past
    if (z == NULL) {
        z = calloc(indice, sizeof(double));
        if (z == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour z\n");
            return EXIT_FAILURE;
        }
    }
    else {
        fprintf(stderr, "Erreur : z n'est pas NULL\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int Destroy_ADMMRDA(int n, int indice) 
{
    if (g_avg != NULL) {
        free(g_avg);
        g_avg = NULL;
    }
    if (mu != NULL) {
        free(mu);
        mu = NULL;
    }
    if (z != NULL) {
        free(z);
        z = NULL;
    }
    if (B != NULL) {
        for (int i = 0; i < indice; i++) {
            if (B[i] != NULL)
                free(B[i]);
        }
        free(B);
        B = NULL;
    }
    if (BT != NULL) {
        for (int i = 0; i < n; i++) {
            if (BT[i] != NULL)
                free(BT[i]);
        }
        free(BT);
        BT = NULL;
    }
    
    return EXIT_SUCCESS;
}



int Learning_ADMMRDA(PreferenceType preference, double *m, double *x, double *x_prime, double y, double y_prime, int indice, int t, double gamma, double lambda, double delta, double rho, int n, int k)
{
    // === DEBUG: Vérification des pointeurs et des paramètres ===
    if (!m || !x || !x_prime) {
        fprintf(stderr, "[ERREUR] Pointeur NULL dans les entrées (m, x, x_prime)\n");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    if (t <= 0) {
        fprintf(stderr, "[ERREUR] t <= 0 (%d)\n", t);
        fflush(stderr);
        return EXIT_FAILURE;
    }

    if (gamma <= 0 || lambda < 0 || rho <= 0) {
        fprintf(stderr, "[ERREUR] Paramètres invalides : gamma=%.10f lambda=%.10f rho=%.10f\n", gamma, lambda, rho);
        fflush(stderr);
        return EXIT_FAILURE;
    }

    // printf(" -> ADMMRDA preference=%d y=%.17f y'=%.17f\n", preference, y, y_prime);
    // fflush(stdout);

    double *g = (double*)calloc(n, sizeof(double));
    if (!g) {
        perror("[ERREUR] Allocation échouée pour g");
        return EXIT_FAILURE;
    }

    // printf("e");

    double delta_y = y - y_prime;

    // Calcul de g
    if (preference == STRICT_PREFERENCE) {
        if (delta_y < delta) {
            for (int i = 0; i < n; i++)
                g[i] = x_prime[i] - x[i];
        } else {
            memset(g, 0, n * sizeof(double));
        }
    } else if (preference == INDIFFERENCE) {
        if (fabs(delta_y) < delta) {
            memset(g, 0, n * sizeof(double));
        } else {
            double signe = (delta_y > 0.0) ? 1.0 : -1.0;
            for (int i = 0; i < n; i++)
                g[i] = signe * (x[i] - x_prime[i]);
        }
    } else {
        fprintf(stderr, "[ERREUR] Type de préférence inconnu : %d\n", preference);
        fflush(stderr);
        free(g);
        return EXIT_FAILURE;
    }

    // Moyenne mobile de gradient
    for (int i = 0; i < n; i++) {
        g_avg[i] += (1.0 / t) * (g[i] - g_avg[i]);
    }

    double *g_avg_mu = (double *)calloc(n, sizeof(double));
    if (!g_avg_mu) {
        perror("[ERREUR] Allocation échouée pour g_avg_mu");
        free(g);
        return EXIT_FAILURE;
    }

    // Étapes 1 à 5
    double *Bm = (double *)calloc(indice, sizeof(double));
    double *Bm_minus_z = (double *)calloc(indice, sizeof(double));
    double *mu_minus_rho_Bm_z = (double *)calloc(indice, sizeof(double));
    double *BT_mu_minus_rho_Bm_z = (double *)calloc(n, sizeof(double));

    if (!Bm || !Bm_minus_z || !mu_minus_rho_Bm_z || !BT_mu_minus_rho_Bm_z) {
        fprintf(stderr, "[ERREUR] Allocation mémoire dans gradient intermédiaire\n");
        fflush(stderr);
        free(g); free(g_avg_mu); free(Bm); free(Bm_minus_z); free(mu_minus_rho_Bm_z); free(BT_mu_minus_rho_Bm_z);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < indice; i++)
        for (int j = 0; j < n; j++)
            Bm[i] += B[i][j] * m[j];

    for (int i = 0; i < indice; i++)
        Bm_minus_z[i] = Bm[i] - z[i];

    for (int i = 0; i < indice; i++)
        mu_minus_rho_Bm_z[i] = mu[i] - rho * Bm_minus_z[i];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < indice; j++)
            BT_mu_minus_rho_Bm_z[i] += BT[i][j] * mu_minus_rho_Bm_z[j];

    for (int i = 0; i < n; i++)
        g_avg_mu[i] = g_avg[i] - BT_mu_minus_rho_Bm_z[i];

    // Mise à jour m
    for (int i = 0; i < n; i++) {
        char signe = (g_avg_mu[i] > 0.0) ? 1 : -1;
        m[i] = -(sqrt(t) / gamma) * fmax(0.0, fabs(g_avg_mu[i]) - lambda) * signe;
    }

    // Étapes 7 et 8
    memset(Bm, 0, indice * sizeof(double));
    for (int i = 0; i < indice; i++)
        for (int j = 0; j < n; j++)
            Bm[i] += B[i][j] * m[j];

    for (int i = 0; i < indice; i++) {
        z[i] = -fmax(0.0, (mu[i] / rho) - Bm[i]);
        mu[i] = mu[i] - rho * (Bm[i] - z[i]);
    }

    free(g);
    free(g_avg_mu);
    free(Bm);
    free(Bm_minus_z);
    free(mu_minus_rho_Bm_z);
    free(BT_mu_minus_rho_Bm_z);

    return EXIT_SUCCESS;
}





// ==================================================================
double Error_ADMMRDA(PreferenceType preference, double y, double y_prime, double delta)
{
    if (preference == STRICT_PREFERENCE) {
        return fabs(fmax(0.0, delta - y + y_prime));
    } 
    else if(preference == INDIFFERENCE) {
        return fabs(fmax(0.0, fabs(y - y_prime) - delta));
    }
    
    return NAN; // Sinon, on a un probleme avec preference
}