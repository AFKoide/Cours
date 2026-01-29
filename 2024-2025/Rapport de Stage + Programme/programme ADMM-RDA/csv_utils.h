#ifndef CSV_UTILS_H
#define CSV_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE 8192
#define MAX_COLS 1024


/**
 * @brief Charge les données d'un fichier CSV, mélange les lignes, et sépare les données en tableaux de caractéristiques et de cibles.
 * 
 * @param path        Chemin vers le fichier CSV à charger.
 * @param col_cible   Index de la colonne cible dans le fichier CSV.
 * @param X_total     [Output] Pointera vers un tableau 2D contenant les valeurs des caractéristiques.
 * @param y_total     [Output] Pointera vers un tableau contenant les valeurs cibles.
 * @param m_total     [Output] Contiendra le nombre de lignes (échantillons) chargées.
 * @param n           [Output] Contiendra le nombre de colonnes de caractéristiques (hors colonne cible).
 * @return            0 en cas de succès, ou un code d'erreur non nul en cas d'échec.
 */
int charger_melanger_csv(const char *path, int col_cible, /*output*/ double ***X_total, double **y_total, int *m_total, int *n) ;



/**
 * @brief Sépare un dataset en ensembles d'entraînement et de test.
 *
 * Cette fonction prend en entrée un dataset complet (X_total, y_total) et le divise en deux sous-ensembles :
 * un ensemble d'entraînement et un ensemble de test, selon les tailles spécifiées.
 *
 * @param X_total     Matrice des caractéristiques de l'ensemble complet (dimensions : m_total x n).
 * @param y_total     Vecteur des étiquettes de l'ensemble complet (taille : m_total).
 * @param m_total     Nombre total d'exemples dans le dataset.
 * @param m_train     Nombre d'exemples à inclure dans l'ensemble d'entraînement.
 * @param m_test      Nombre d'exemples à inclure dans l'ensemble de test.
 * @param X_train     [Sortie] Pointeur vers la matrice des caractéristiques de l'ensemble d'entraînement (dimensions : m_train x n).
 * @param y_train     [Sortie] Pointeur vers le vecteur des étiquettes de l'ensemble d'entraînement (taille : m_train).
 * @param X_test      [Sortie] Pointeur vers la matrice des caractéristiques de l'ensemble de test (dimensions : m_test x n).
 * @param y_test      [Sortie] Pointeur vers le vecteur des étiquettes de l'ensemble de test (taille : m_test).
 *
 * @return            0 en cas de succès, une valeur négative en cas d'erreur (par exemple, si les tailles ne correspondent pas).
 */
int separer_dataset(double **X_total, double *y_total, int m_total, int m_train, int m_test, /*output*/ double ***X_train, double **y_train, double ***X_test, double **y_test) ;


/**
 * @brief Libère la mémoire allouée pour un dataset (X, y).
 *
 * @param X Tableau 2D des caractéristiques à libérer.
 * @param y Tableau des valeurs cibles à libérer.
 * @param m Nombre de lignes (exemples) dans le dataset.
 */
void liberer_dataset(double **X, double *y, int m);



#endif // CSV_UTILS_H

// CURTIS CONTROL EST PASSÉ ICI
/*⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⠤⠶⠶⠶⠤⠤⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀ ⠀⠀⠀⢀⣠⠶⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠳⢦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⣰⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⣰⠏⠀⢠⣶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⡀⠀⠹⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⢠⡏⠀⢠⠟⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⢳⡄⠀⢹⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⣾⠀⠀⡞⠀⢸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠇⠀⢷⠀⠈⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⢠⡇⠀⢠⠇⠀⠀⠙⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠋⠀⠀⠸⡆⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⢸⡇⠀⢸⠀⠀⠀⠀⠙⢦⡀⠀⠀⠀⠀⠀⠀⢀⡾⠃⠀⠀⠀⢰⠇⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⢸⡇⠀⢸⠀⠀⠀⠀⠀⠀⠳⣄⣰⡄⢰⣄⣰⠋⠀⠀⠀⠀⠀⢸⡀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠘⡇⠀⢸⠀⠀⠀⠀⠀⠀⠀⠈⢹⡇⢸⠉⠁⠀⠀⠀⠀⠀⠀⠈⡇⠀⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⢿⠀⠈⢧⡀⠀⠀⠀⠀⠀⠀⣾⠀⢸⣄⠀⠀⠀⠀⠀⠀⠀⡼⢁⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⢸⠘⣆⠈⢧⠀⠀⠀⠀⣠⠴⠃⠀⠀⠙⢧⣀⠀⠀⠀⠀⡼⠀⡼⢸⡃⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀
 * ⠀⣾⠀⢸⣆⠘⠷⠤⠴⠚⣁⣀⣤⣤⣤⣄⣀⠉⠙⠲⠴⠚⢡⣾⠇⠘⡇⠀⠀⠀⠀⠀⠈⢷⣤⡀⠀
 * ⠀⢹⣆⠈⣿⣷⣾⡖⣾⠻⣟⣾⣏⣻⣹⡟⡟⣿⢳⣶⣶⣾⣿⡟⠀⢰⡇⠀⠀⠀⠀⠀⠀⠘⣇⢻⡄
 * ⠀⠀⠻⣄⢸⣿⡋⠹⠿⣿⣿⡿⢿⣿⡟⣿⢿⣿⡟⠹⠋⠙⡿⠁⣴⠟⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⣷
 * ⠀⠀⠀⠹⣶⣿⣿⣄⠀⢻⣘⣦⣼⣿⣴⣿⣼⣼⣇⡀⣰⣿⠃⣴⠏⠀⠀⠀⠀⠀⠀⠀⢀⣴⠏⢀⡏
 * ⠀⠀⠀⠀⠙⣿⢿⣿⣥⣾⠛⠉⠀⠀⠉⠙⢦⡀⠈⠙⢿⣇⣰⠃⠀⠀⠀⠀⣀⣀⡤⠶⠛⠁⢠⡾⠁
 * ⠀⠀⠀⠀⠀⠘⣿⠻⣿⢻⣦⠀⢀⡀⠀⠀⠀⠙⠀⠀⠀⠹⣇⠀⠀⢀⣴⠟⠉⠀⠀⢀⣠⡼⠋⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠘⣿⢹⡾⢻⣇⣴⡿⠳⢦⣀⠀⠀⠀⠀⠀⠙⠳⠶⠛⠁⠀⠀⢀⡼⠋⠁⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠀⠘⣿⠻⡎⢹⢿⣷⣶⣎⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠁⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠀⠀⠸⣆⠙⢾⣸⠙⡿⢿⣩⠏⢹⣦⣀⠀⠀⠀⠀⠀⢀⣴⠟⠁⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣦⡀⠉⠉⠉⠉⠁⣰⠏⠀⠈⠉⠛⠒⠒⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠓⠒⠒⠒⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/