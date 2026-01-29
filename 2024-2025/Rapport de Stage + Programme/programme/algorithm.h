#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "AlgoPhixGame.h"
#include "equations.h"

typedef struct {
    double erreur_moy;
    int nb_indifference;
    int nb_preference;
    int nb_indifference_vraie;
    int nb_indifference_fausse;
    int nb_preference_vraie;
    int nb_preference_fausse;
    int nb_preference_inverse;
} ResultatsAlgo;

double gaussianRandom();
double ProduitScalaire(double *x, double *y, int n);


/**
 * @brief Applique un algorithme d'apprentissage sur une base de données.
 *
 * Cette fonction effectue l'apprentissage d'un modèle à partir d'une base de données d'apprentissage.
 *
 * @param database_learning_x   Matrice des variables explicatives (taille n x m).
 * @param database_learning_y   Vecteur des variables à prédire (taille n).
 * @param var_phi               Vecteur des paramètres du modèle (taille m ou k selon le contexte).
 * @param epoch                 Nombre d'époques d'apprentissage.
 * @param n                     Nombre d'exemples dans la base de données.
 * @param m                     Nombre de variables explicatives.
 * @param k                     Paramètre supplémentaire (ex : nombre de classes ou de sorties).
 * @param iteration_apprentissage Nombre maximal d'itérations par apprentissage.
 * @param beta                  Taux d'apprentissage ou paramètre de régularisation.
 * @param gamma                 Paramètre de régularisation ou d'ajustement.
 * @param delta                 Seuil de variation pour la convergence.
 * @param tolerance             Tolérance pour le critère d'arrêt.
 * @param use_L1                Indique si la régularisation L1 est utilisée (1 : oui, 0 : non).
 * @param method                Méthode d'apprentissage à utiliser (identifiant).
 * 
 * @return                      Erreur moyenne a la fin après apprentissage
 */
ResultatsAlgo Algorithm_learning(double **database_learning_x, double *database_learning_y, double *var_phi, int epoch, int n, int m, int k, int iteration_apprentissage, double err_precedente, double beta, double delta, double gamma, double tolerance, int use_L1, int method, int ordre);



/**
 * @brief Calcule l'erreur moyenne entre les valeurs prédites et les valeurs réelles.
 *
 * Cette fonction évalue l'erreur moyenne (L1 ou L2) entre les sorties prédites par un modèle
 * (définies par var_phi) et les valeurs réelles (database_y) pour un ensemble d'échantillons.
 *
 * @param database_x    Tableau 2D contenant les variables d'entrée pour chaque échantillon.
 * @param database_y    Tableau contenant les valeurs de sortie réelles pour chaque échantillon.
 * @param var_phi       Tableau contenant les paramètres ou variables du modèle utilisé pour la prédiction.
 * @param tolerance     Tolérance utilisée dans le calcul de l'erreur (peut servir à ignorer de petites erreurs).
 * @param delta         Paramètre supplémentaire pouvant être utilisé selon la méthode d'erreur choisie.
 * @param nb_samples    Nombre total d'échantillons dans la base de données.
 * @param m             Nombre de variables d'entrée (features) par échantillon.
 * @param n             Taille du vecteur var_phi (nombre de paramètres du modèle).
 * @param use_L1        Si différent de zéro, utilise la norme L1 (erreur absolue), sinon utilise la norme L2 (erreur quadratique).
 * @param method        Indique la méthode spécifique de calcul de l'erreur à utiliser.
 * @param k             Paramètre supplémentaire pouvant être utilisé selon la méthode choisie.
 * 
 * @return              La valeur de l'erreur moyenne calculée.
 */
double Error_mean(double **database_x, 
                  double *database_y, 
                  double *var_phi, 
                  double tolerance, double delta, 
                  int nb_samples, int m, int n, 
                  int use_L1, int method, 
                  int k);



/**
 * @brief Teste l'algorithme spécifié sur un ensemble de données de test.
 *
 * Cette fonction évalue les performances d'un algorithme à l'aide de l'ensemble de test fourni.
 *
 * @param database_test_x   Pointeur vers un tableau 2D contenant les données d'entrée de test (caractéristiques).
 * @param database_test_y   Pointeur vers un tableau contenant les sorties de test (étiquettes/cibles).
 * @param var_phi           Pointeur vers un tableau contenant les paramètres ou variables spécifiques à l'algorithme.
 * @param n                 Nombre d'échantillons de test (lignes dans database_test_x).
 * @param m                 Nombre de caractéristiques par échantillon (colonnes dans database_test_x).
 * @param k                 Paramètre spécifique à l'algorithme (par exemple : nombre de classes ou de clusters).
 * @param iteration_test    Nombre d'itérations à effectuer lors du test.
 * @param tolerance         Valeur de tolérance pour la convergence ou le critère d'arrêt.
 * @param delta             Paramètre supplémentaire spécifique à l'algorithme (par exemple : taux d'apprentissage ou seuil).
 * @param method            Entier spécifiant l'algorithme ou la méthode à utiliser.
 * @param logf2             Pointeur de fichier pour la journalisation des résultats et informations de test.
 * @param use_L1            Booléen qui détermine si on utilise ou non la méthode L1
 * 
 * @return                  Retourne l'erreur moyenne à la suite du test
 */
ResultatsAlgo Algorithm_test(double **database_test_x, double *database_test_y, double *var_phi, int n, int m, int k, int iteration_test, double err_precedente, double tolerance, double delta, int method, int use_L1, int ordre);





ResultatsAlgo Algorithm_Test_Imprecis(double **database_test_x, double *database_test_y, double *var_phi, int n, int m, int k, int iteration_test, double err_precedente, double tolerance, double delta, int method, int use_L1, int ordre) ;


double Error_Mean_Imprecis(
    double **database_x, double *database_y, double *var_phi,
    double tolerance, int nb_samples, int m, int n, int k, int method, int ordre, int use_L1
);


ResultatsAlgo Algorithm_Learning_Imprecis(double **database_learning_x, double *database_learning_y, double *var_phi, int epoch, int n, int m, int k, int iteration_apprentissage, double err_precedente, double beta, double delta, double gamma, double tolerance, int use_L1, int method, int ordre) ;





/**
 * @brief Algorithme principal pour le traitement des ensembles de données d'apprentissage et de test.
 *
 * Cette fonction réalise les opérations algorithmiques principales sur les ensembles de données d'apprentissage et de test fournis.
 * Elle prend en charge différents modes et options de régularisation, et permet l'ajustement des paramètres via ses arguments.
 *
 * @param database_learning_x       Pointeur vers le tableau 2D contenant les caractéristiques de l'ensemble d'apprentissage (taille : m_learning x n).
 * @param database_learning_y       Pointeur vers le tableau contenant les étiquettes/cibles de l'ensemble d'apprentissage (taille : m_learning).
 * @param m_learning                Nombre d'échantillons dans l'ensemble d'apprentissage.
 * @param database_test_x           Pointeur vers le tableau 2D contenant les caractéristiques de l'ensemble de test (taille : m_test x n).
 * @param database_test_y           Pointeur vers le tableau contenant les étiquettes/cibles de l'ensemble de test (taille : m_test).
 * @param m_test                    Nombre d'échantillons dans l'ensemble de test.
 * @param n                         Nombre de caractéristiques par échantillon.
 * @param k                         Paramètre pour l'algorithme (par exemple : nombre de clusters, voisins ou composantes selon la méthode).
 * @param iteration                 Nombre d'itérations principales à effectuer.
 * @param iteration_apprentissage   Nombre d'itérations pour la phase d'apprentissage.
 * @param iteration_test            Nombre d'itérations pour la phase de test.
 * @param beta                      Paramètre de régularisation ou spécifique à l'algorithme.
 * @param gamma                     Paramètre de régularisation ou spécifique à l'algorithme.
 * @param delta                     Paramètre de régularisation ou spécifique à l'algorithme.
 * @param tolerance                 Tolérance de convergence pour les algorithmes itératifs.
 * @param use_L1                    Si différent de zéro, utilise la régularisation L1 ; sinon, utilise L2 ou aucune régularisation.
 * @param method                    Entier spécifiant la méthode algorithmique à utiliser.
 * 
 * @return                          int Code de statut (0 si succès, différent de zéro en cas d'erreur).
 */
int main_algo(double **database_learning_x, double *database_learning_y, int m_learning, 
              double **database_test_x, double *database_test_y, int m_test, int n, 
              int k, 
              int iteration, int iteration_apprentissage, int iteration_test, 
              double beta, double gamma, double delta, double tolerance, 
              int use_L1, int method, int ordre);





#endif /* ALGORITHM_H */

/*
*  ┌┐   ┌┐    
* ┌┘┴───┘┴┐   
* │       │   
* │  ─ ─  │   
* │ ┬┘ └┬ │   
* │       │   
* │   ┴   │   
* │       │   
* └─┐   ┌─┘   
*   │   │     MAY THE DIVINE BEAST
*   │   │     SHIELD US FROM BUGS!
*   │   └────┐ 
*   │        ├┐
*   │        ┌┘
*   └┐┐┌──┬┐┌┘ 
*    │┤┤  │┤┤  
*    └┴┘  └┴┘  
*/
