#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <stddef.h> // For size_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "AlgoPhixGame.h"

// Déclarations des fonctions pour le fichier equations.c

/**
 * @brief Met à jour le vecteur de poids var_phi selon la méthode L1.
 *
 * Cette fonction applique la mise à jour L₁ basée sur la moyenne glissante
 * des gradients Γ, avec régularisation paramétrée par γ.
 *
 * @param egalite    Non‐nul si les deux échantillons sont considérés égaux (|y - y′| ≤ δ), sinon 0.
 * @param var_phi    Vecteur de poids à mettre à jour (taille n).
 * @param phi        Vecteur φ(x) calculé pour le premier échantillon (taille n).
 * @param phi_prime  Vecteur φ(x′) calculé pour le second échantillon (taille n).
 * @param y          Valeur agrégée y(x) du premier échantillon (produit scalaire).
 * @param y_prime    Valeur agrégée y(x′) du second échantillon.
 * @param t          Numéro d’itération courant (utilisé pour la moyenne glissante).
 * @param beta       Paramètre β contrôlant le taux d'apprentissage.
 * @param delta      Seuil δ de tolérance pour décider de la correction.
 * @param gamma      Paramètre γ pour le taux de décroissance du pas.
 * @param n          Dimension des vecteurs var_phi, phi et phi_prime.
 */
void L1_Update( int egalite,
                double *var_phi, 
                double *phi,
                double *phi_prime,
                double y,
                double y_prime,
                int t,
                double beta,
                double delta,
                double gamma,
                int n);

/**
 * @brief Calcule l’erreur pour une paire avec la méthode L1.
 *
 * Le critère L1 pénalise selon :
 *   • ([δ − (y − y′)]⁺)   si y > y′
 *   • ([|y − y′| − δ]⁺)   sinon
 *
 * où [u]⁺ = max(0, u).
 *
 * @param egalite    Non‐nul si les deux échantillons sont considérés égaux (|y - y′| ≤ δ), sinon 0.
 * @param y          Valeur agrégée y(x) du premier échantillon.
 * @param y_prime    Valeur agrégée y(x′) du second échantillon.
 * @param delta      Seuil δ de tolérance.
 * @return           Valeur de l’erreur L1 (double).
 */
double L1_Error(int egalite,
        double y,
        double y_prime,
        double delta);

/**
 * @brief Met à jour le vecteur de poids var_phi selon la méthode L2.
 *
 * Cette fonction applique une correction de type carré L₂ :
 *   • si y > y′ + δ, on corrige par β·(φ′ − φ)·(δ − (y − y′))
 *   • si |y − y′| > δ, on corrige par β·sign(y − y′)·(φ − φ′)·(|y − y′| − δ)
 *
 * @param egalite    Non‐nul si |y − y′| ≤ δ, sinon 0.
 * @param var_phi    Vecteur de poids à mettre à jour (taille n).
 * @param phi        Vecteur φ(x) calculé pour le premier échantillon (taille n).
 * @param phi_prime  Vecteur φ(x′) calculé pour le second échantillon (taille n).
 * @param y          Valeur agrégée y(x) du premier échantillon (produit scalaire).
 * @param y_prime    Valeur agrégée y(x′) du second échantillon.
 * @param beta       Paramètre β contrôlant l’amplitude de la mise à jour.
 * @param delta      Seuil δ de tolérance pour décider de la correction.
 * @param n          Dimension des vecteurs var_phi, phi et phi_prime.
 */
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
         int n);

/**
 * @brief Calcule l’erreur pour une paire avec la méthode L2.
 *
 * Le critère L2 pénalise selon :
 *   • (max(0, δ − (y − y′)))²    si y > y′
 *   • (max(0, |y − y′| − δ))²    sinon
 *
 * @param egalite    Non‐nul si |y − y′| ≤ δ, sinon 0.
 * @param y          Valeur agrégée y(x) du premier échantillon.
 * @param y_prime    Valeur agrégée y(x′) du second échantillon.
 * @param delta      Seuil δ de tolérance.
 * @return           Valeur de l’erreur L2 (double).
 */
double L2_Error(int egalite,
        double y,
        double y_prime,
        double delta);













int L2_Update_Imprecis_FAIBLE(
        double *var_phi,
        double *q_phi_inf, double *q_phi_sup,
        double *q_phi_prime_inf, double *q_phi_prime_sup,
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai,
        double learning_rate, double gamma,
        int n, int epoch);

double L2_Error_Imprecis_FAIBLE(
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai);

int L2_Update_Imprecis_FORT(
        double *var_phi,
        double *q_phi_inf, double *q_phi_sup,
        double *q_phi_prime_inf, double *q_phi_prime_sup,
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai,
        double learning_rate, double gamma,
        int n, int epoch);

double L2_Error_Imprecis_FORT(
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai);

int L1_Update_Imprecis_FAIBLE(
        double *var_phi,
        double *q_phi_inf, double *q_phi_sup,
        double *q_phi_prime_inf, double *q_phi_prime_sup,
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai,
        double learning_rate, double gamma,
        int n, int epoch);

double L1_Error_Imprecis_FAIBLE(
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai);

int L1_Update_Imprecis_FORT(
        double *var_phi,
        double *q_phi_inf, double *q_phi_sup,
        double *q_phi_prime_inf, double *q_phi_prime_sup,
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai,
        double learning_rate, double gamma,
        int n, int epoch);

double L1_Error_Imprecis_FORT(
        double y_inf, double y_sup,
        double y_prime_inf, double y_prime_sup,
        char indifference_vrai);





#endif // EQUATIONS_H