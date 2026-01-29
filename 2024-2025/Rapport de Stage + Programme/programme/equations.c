#include "equations.h"

#define FORT                1
#define FAIBLE              0
#define BUFFER              fflush(stdout)


// stockage de l’état pour L1_Update
double *Γ_past = NULL;
int     Γ_size = 0;


char Initialise_Γ_past(int n) 
{
    if (n <= 0) {
        fprintf(stderr, "Erreur: taille n invalide (%d) pour Γ_past\n", n);
        return EXIT_FAILURE;
    }

    // Création de Γ_past si n'existe pas, puis mise à jour de Γ_past
    if (Γ_past == NULL || Γ_size != n) {
        if (Γ_past != NULL) {
            printf("Libération de l'ancien Γ_past de taille %d\n", Γ_size);
            free(Γ_past);
        }
        Γ_past = malloc(n * sizeof(double));
        if (Γ_past == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour Γ_past\n");
            return EXIT_FAILURE;
        }
        for (int i = 0; i < n; i++)
            Γ_past[i] = 0.0;
        Γ_size = n;
    }

    return EXIT_SUCCESS;
}

char Update_L1(int n, double *Γ, double gamma, double beta, int t, /*output*/ double *var_phi)
{
    // Mise à jour Γ_past
    for (int i = 0; i < n; i++)
        Γ_past[i] = Γ_past[i] + (1/t) * (Γ[i] - Γ_past[i]);
    
    int signe;

    for (int i = 0; i < n; i++) {
        signe = (Γ_past[i] >= 0.0) ? 1 : -1;
        var_phi[i] = signe * (sqrt(t)/gamma) * fmax(0.0, fabs(Γ_past[i] - beta));
    }

    return EXIT_SUCCESS;
}



// --------------------------------------------------------------------------------
// L2_Update : met à jour des poids avec la méthode L2
// --------------------------------------------------------------------------------
void L2_Update(int egalite,
               double *var_phi,
               double *phi,
               double *phi_prime,
               double y,
               double y_prime,
               int epoch,
               double beta,
               double delta,
               double gamma,
               int n)
{
    (void) gamma;
    (void) epoch;
        double delta_y = y - y_prime;

    if (!egalite) {
        double m = fmax(0.0, delta - delta_y);
        for(int i = 0; i < n; i++)
            var_phi[i] -= beta * (phi_prime[i] - phi[i]) * m;
    } else {
        double signe = (delta_y < 0.0) ? -1.0 : 1.0;
        double m = fmax(0.0, fabs(delta_y) - delta);
        for(int i = 0; i < n; i++)
            var_phi[i] -= beta * signe * (phi[i] - phi_prime[i]) * m;
    }
}



int L2_Update_Imprecis_FAIBLE(
    double *var_phi,
    double *q_phi_inf, double *q_phi_sup,
    double *q_phi_prime_inf, double *q_phi_prime_sup,
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai,
    double learning_rate, double gamma,
    int n,
    int epoch)
{
    (void) epoch;
    (void) gamma;
    if (indifference_vrai) {
        if ( ((y_sup - y_prime_sup) * (y_inf - y_prime_inf)) > 0 ) 
        {
            if (fabs(y_sup - y_prime_sup) <= fabs(y_inf - y_prime_inf)) {
            // if (fabs(y_sup - y_prime_sup) >= fabs(y_inf - y_prime_inf)) {
                for (int i = 0; i < n; i++)
                    var_phi[i] -= learning_rate * (q_phi_inf[i] - q_phi_prime_inf[i]) * (y_inf - y_prime_inf);
            } 
            else {
                for (int i = 0; i < n; i++)
                    var_phi[i] -= learning_rate *
                                (q_phi_sup[i] - q_phi_prime_sup[i]) *
                                (y_sup - y_prime_sup);
            }
        }
    }
    else {
        for (int i = 0; i < n; i++) {
            var_phi[i] -= learning_rate * ((q_phi_prime_inf[i] - q_phi_inf[i]) * fmax(0.0, y_prime_inf - y_inf) +
                                           (q_phi_prime_sup[i] - q_phi_sup[i]) * fmax(0.0, y_prime_sup - y_sup));
        }
    }

    return EXIT_SUCCESS;
}



int L2_Update_Imprecis_FORT(
    double *var_phi,
    double *q_phi_inf, double *q_phi_sup,
    double *q_phi_prime_inf, double *q_phi_prime_sup,
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai,
    double learning_rate, double gamma,
    int n,
    int epoch)
{
    (void) epoch;
    (void) gamma;
    if(indifference_vrai) {
        if (((y_sup - y_prime_inf) * (y_inf - y_prime_sup)) <= 0.0) {
            if(fabs(y_sup - y_prime_inf) <= fabs(y_inf - y_prime_sup)) {
                for (int i = 0; i < n; i++) {
                    var_phi[i] -= learning_rate * 
                                    (((q_phi_inf[i] - q_phi_prime_sup[i]) /* /2 */) * (y_inf - y_prime_sup));
                }
            }
            else {
                for (int i = 0; i < n; i++) {
                    var_phi[i] -= learning_rate * 
                                    (((q_phi_sup[i] - q_phi_prime_inf[i]) /* /2 */) * (y_sup - y_prime_inf));
                }
            }
        }
    }
    else {
        for (int i = 0; i < n; i++) {
            var_phi[i] -= learning_rate * 
                            ((q_phi_prime_sup[i] - q_phi_inf[i]) /* /2 */) * fmax(0.0, y_prime_sup - y_inf);
        }
    }

    return EXIT_SUCCESS;
}




// --------------------------------------------------------------------------------
// L2_Error : calcule l’erreur avec la méthode L2
// --------------------------------------------------------------------------------
double L2_Error(int egalite,
                double y,
                double y_prime,
                double delta)
{
    if (!egalite) {
        return pow(fmax(0.0, delta - y + y_prime), 2);
    } else {
        return pow(fmax(0.0, fabs(y - y_prime) - delta), 2);
    }
}



double L2_Error_Imprecis_FAIBLE(
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai)
{
    double error;

    if(indifference_vrai) {
        if((y_inf - y_prime_inf) * (y_sup - y_prime_sup) <= 0) {
            error = 0;
        }
        else {
            // error = fmin(pow(y_inf - y_prime_inf, 2), pow(y_sup - y_prime_inf, 2));        // AVEC CETTE ERREUR, UPDATE DOIT ETRE SUP >= INF
            error = fmax(pow(y_inf - y_prime_inf, 2), pow(y_sup - y_prime_sup, 2));      // AVEC CETTE ERREUR, UPDATE DOIT ETRE SUP <= INF
        }
    }
    else {
        error = pow(fmax(0.0, y_prime_inf - y_inf), 2) + pow(fmax(0.0, y_prime_sup - y_sup), 2);
    }

    return error;
}



double L2_Error_Imprecis_FORT(
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai)
{
    double error;

    if(indifference_vrai) {
        if((y_inf - y_prime_sup) * (y_sup - y_prime_inf) <= 0) {
            error = 0;
        }
        else {
            error = fmax(pow(y_inf - y_prime_sup, 2), pow(y_sup - y_prime_inf, 2));
        }
    }
    else {
        error = pow(fmax(0.0, y_prime_sup - y_inf), 2);
    }

    return error;
}





// --------------------------------------------------------------------------------
// L1_Update : Mise à jour des poids avec la méthode L1
// --------------------------------------------------------------------------------
void L1_Update(int egalite,
               double *var_phi, 
               double *phi,
               double *phi_prime,
               double y,
               double y_prime,
               int t,
               double beta,
               double delta,
               double gamma,
               int n)
{
    // (re)initialisation de Γ_past
    Initialise_Γ_past(n);

    double delta_y = y - y_prime;
    double *Γ = malloc(n * sizeof(double));

    // calcul de Γ selon egalite et D
    if (!egalite) {
        if (delta_y < delta) {
            for(int i = 0; i < n; i++)
                Γ[i] = phi_prime[i] - phi[i];
        } 
        else {
            memset(Γ, 0, n * sizeof(double));
        }
    } else {
        if (fabs(delta_y) < delta) {
            memset(Γ, 0, n * sizeof(double));
        } 
        else {
            double signe = (delta_y > 0.0) ? 1.0 : -1.0;
            for(int i = 0; i < n; i++)
                Γ[i] = signe * (phi[i] - phi_prime[i]);
        }
    }

    // mise à jour de Γ_past
    for(int i = 0; i < n; i++) {
        Γ_past[i] += (1.0 / t) * (Γ[i] - Γ_past[i]);
    }

    // mise à jour de var_phi
    for(int i = 0; i < n; i++) {
        double g = Γ_past[i];
        double step = (sqrt(t) / gamma) * fmax(0.0, fabs(g) - beta);
        var_phi[i] = -((g >= 0.0) ? 1.0 : -1.0) * step;
    }

    // libération du buffer
    free(Γ);
}



int L1_Update_Imprecis_FAIBLE(
    double *var_phi,
    double *q_phi_inf, double *q_phi_sup,
    double *q_phi_prime_inf, double *q_phi_prime_sup,
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai,
    double beta, double gamma,
    int n, int epoch)
{
    if (Initialise_Γ_past(n) == EXIT_FAILURE) {
        free(Γ_past);
        return EXIT_FAILURE;
    }

    double *Γ = (double *)malloc(n * sizeof(double));


    if(indifference_vrai) {
        if(((y_sup - y_prime_sup) * (y_inf - y_prime_inf)) <= 0.0)
            memset(Γ, 0.0, n * sizeof(double));
        
        else if(fabs(y_sup - y_prime_inf) <= fabs(y_inf - y_prime_sup)) {
            for(int i = 0; i < n; i++) {
                Γ[i] = q_phi_inf[i] - q_phi_prime_inf[i];
                Γ[i] *= ((y_inf - y_prime_inf) >= 0) ? 1 : -1;
            }
        }
        else {
            for(int i = 0; i < n; i++) { 
                Γ[i] = q_phi_sup[i] - q_phi_prime_sup[i];
                Γ[i] *= ((y_sup - y_prime_sup) >= 0) ? 1 : -1;
            }
        }
    }
    else {
        for(int i = 0; i < n; i++) {
            double X1 = ((y_prime_inf - y_inf) > 0) ? (y_prime_inf - y_inf) : 0.0;
            double X2 = ((y_prime_sup - y_sup) > 0) ? (y_prime_sup - y_sup) : 0.0;

            Γ[i] = (q_phi_prime_inf - q_phi_inf) * X1 + (q_phi_prime_sup - q_phi_inf) * X2;
        }
    }

    Update_L1(n, Γ, gamma, beta, epoch, var_phi);

    return EXIT_SUCCESS;
}



int L1_Update_Imprecis_FORT(
    double *var_phi,
    double *q_phi_inf, double *q_phi_sup,
    double *q_phi_prime_inf, double *q_phi_prime_sup,
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai,
    double beta, double gamma,
    int n, int epoch)
{
    Initialise_Γ_past(n); 

    double *Γ = (double *)malloc(n * sizeof(double));


    if(indifference_vrai) {
        if(((y_sup - y_prime_inf) * (y_inf - y_prime_sup)) <= 0.0)
            memset(Γ, 0, n * sizeof(double));

        else {
            if(fabs(y_sup - y_prime_inf) >= fabs(y_inf - y_prime_sup)) {
                for(int i = 0; i < n; i++) 
                    Γ[i] = (q_phi_sup[i] - q_phi_prime_inf[i]) /* /2 */;
            }
            else {
                for(int i = 0; i < n; i++) 
                    Γ[i] = (q_phi_inf[i] - q_phi_prime_sup[i]) /* /2 */;
            }
        }
    }
    else {
        for(int i = 0; i < n; i++) 
            Γ[i] = (fmax(0.0, q_phi_prime_sup[i] - q_phi_inf[i])) /* /2 */;
    }

    Update_L1(n, Γ, gamma, beta, epoch, var_phi);

    return EXIT_SUCCESS;
}



// --------------------------------------------------------------------------------
// L1_Error : calcule l’erreur avec la méthode L1
// --------------------------------------------------------------------------------
double L1_Error(int egalite,
                double y,
                double y_prime,
                double delta)
{
    if (!egalite) {
        return fabs(fmax(0.0, delta - y + y_prime));
    } else {
        return fabs(fmax(0.0, fabs(y - y_prime) - delta));
    }
}



double L1_Error_Imprecis_FAIBLE(
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai)
{
    double error;

    if(indifference_vrai) {
        if(((y_sup - y_prime_sup) * (y_inf - y_prime_inf)) <= 0)
            error = 0.0;
        else
            error = fmax(fabs(y_inf - y_prime_inf), fabs(y_sup - y_prime_sup));
    }
    else {
        error = fmax(0.0, y_prime_inf - y_inf) + fmax(0.0, y_prime_sup - y_sup);
    }

    return error;
}



double L1_Error_Imprecis_FORT(
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai)
{
    double error;
    if(indifference_vrai) {
        if(((y_inf - y_prime_sup)*(y_sup - y_prime_inf)) <= 0)
            error = 0;
        else 
            error = fmax(fabs(y_inf - y_prime_sup), fabs(y_sup - y_prime_inf));
    }
    else {
        error = fmax(0.0, y_prime_sup - y_inf);
    }

    return error;
}


/*
int L1_Update_Imprecis_FAIBLE(
    double *var_phi,
    double *q_phi_inf, double *q_phi_sup,
    double *q_phi_prime_inf, double *q_phi_prime_sup,
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai,
    double beta, double gamma,
    int n, int epoch)
{
    if (Initialise_Γ_past(n) == EXIT_FAILURE) {
        free(Γ_past);
        return EXIT_FAILURE;
    }

    double *Γ = (double *)malloc(n * sizeof(double));


    if(indifference_vrai) {
        if(((y_sup - y_prime_sup) * (y_inf - y_prime_inf)) <= 0.0)
            memset(Γ, 0.0, n * sizeof(double));
        
        else if(fabs(y_sup - y_prime_inf) <= fabs(y_inf - y_prime_sup)) {
            for(int i = 0; i < n; i++) {
                Γ[i] = q_phi_inf[i] - q_phi_prime_inf[i];
                Γ[i] *= ((y_inf - y_prime_inf) >= 0) ? 1 : -1;
            }
        }
        else {
            for(int i = 0; i < n; i++) { 
                Γ[i] = q_phi_sup[i] - q_phi_prime_sup[i];
                Γ[i] *= ((y_sup - y_prime_sup) >= 0) ? 1 : -1;
            }
        }
    }
    else {
        for(int i = 0; i < n; i++) {
            double X1 = ((y_prime_inf - y_inf) > 0) ? (y_prime_inf - y_inf) : 0.0;
            double X2 = ((y_prime_sup - y_sup) > 0) ? (y_prime_sup - y_sup) : 0.0;

            Γ[i] = (q_phi_prime_inf - q_phi_inf) * X1 + (q_phi_prime_sup - q_phi_inf) * X2;
        }
    }

    Update_L1(n, Γ, gamma, beta, epoch, var_phi);

    return EXIT_SUCCESS;
}



int L1_Update_Imprecis_FORT(
    double *var_phi,
    double *q_phi_inf, double *q_phi_sup,
    double *q_phi_prime_inf, double *q_phi_prime_sup,
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai,
    double beta, double gamma,
    int n, int epoch)
{
    Initialise_Γ_past(n); 

    double *Γ = (double *)malloc(n * sizeof(double));


    if(indifference_vrai) {
        if(((y_sup - y_prime_inf) * (y_inf - y_prime_sup)) <= 0.0)
            memset(Γ, 0, n * sizeof(double));

        else {
            if(fabs(y_sup - y_prime_inf) >= fabs(y_inf - y_prime_sup)) {
                for(int i = 0; i < n; i++) 
                    Γ[i] = q_phi_sup[i] - q_phi_prime_inf[i];
            }
            else {
                for(int i = 0; i < n; i++) 
                    Γ[i] = q_phi_inf[i] - q_phi_prime_sup[i];
            }
        }
    }
    else {
        for(int i = 0; i < n; i++) 
            Γ[i] = fmax(0, q_phi_prime_sup[i] - q_phi_inf[i]);
    }

    Update_L1(n, Γ, gamma, beta, epoch, var_phi);

    return EXIT_SUCCESS;
}



// --------------------------------------------------------------------------------
// L1_Error : calcule l’erreur avec la méthode L1
// --------------------------------------------------------------------------------
double L1_Error(int egalite,
                double y,
                double y_prime,
                double delta)
{
    if (!egalite) {
        return fabs(fmax(0.0, delta - y + y_prime));
    } else {
        return fabs(fmax(0.0, fabs(y - y_prime) - delta));
    }
}



double L1_Error_Imprecis_FAIBLE(
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai)
{
    double error;

    if(indifference_vrai) {
        if(((y_sup - y_prime_sup) * (y_inf - y_prime_inf)) <= 0)
            error = 0.0;
        else
            error = fmax(fabs(y_inf - y_prime_inf), fabs(y_sup - y_prime_sup));
    }
    else {
        error = fmax(0, y_prime_inf - y_inf) + fmax(0, y_prime_sup - y_sup);
    }

    return error;
}



double L1_Error_Imprecis_FORT(
    double y_inf, double y_sup,
    double y_prime_inf, double y_prime_sup,
    char indifference_vrai)
{
    double error;
    if(indifference_vrai) {
        if(((y_inf - y_prime_sup)*(y_sup - y_prime_inf)) <= 0)
            error = 0;
        else 
            error = fmax(fabs(y_inf - y_prime_sup), fabs(y_sup - y_prime_inf));
    }
    else {
        error = fmax(0, y_prime_sup - y_inf);
    }

    return error;
}
*/