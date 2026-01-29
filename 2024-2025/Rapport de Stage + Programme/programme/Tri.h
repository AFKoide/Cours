
#ifndef _Tri_H_
#define _Tri_H_

#include <math.h>
#include <stdio.h>

/**********************************************************/
/***************** Fonctions de tri ***********************/
/***** Olivier STRAUSS 1997 ** LIRMM **********************/
/**********************************************************/
/**** Toutes les fonctions sont surchargees pour **********/
/**** fonctionner sur des tableaux de double, int *********/
/**** et unsigned char ************************************/
/**********************************************************/
/**********************************************************/


/************************** INVERSE SENS **************************************/

void Inverse_sens(double *Entree, int nombre_d_elements) ;
void Inverse_sensInt(int *Entree, int nombre_d_elements) ;

/********************** FIN DE INVERSE SENS ***********************************/


/************************** QUICK SORT ****************************************/

int Q_Partitionner( double *A, int initial, int final ) ;

int Q_Tri_Rapide(double *A, int *initial, int *final) ;

int Quick_Sort(double *Entree, int nombre_d_elements, char sens) ;

/************************* FIN DE QUICK SORT **********************************/


/************************** QUICK SORT INDICE *********************************/

int QI_Partitionner( double *Entree, int *Indice, int nombre_d_elements ) ;

int QI_Tri_Rapide(double *A, int *I, int n) ;

int Un_Quick_Sort_Indice(double *Entree, int *Indice, int nombre_d_elements, char sens) ;

int Quick_Sort_Indice(double *Entree, int *Indice, int nombre_d_elements, char sens) ;

/************************** FIN DE QUICK SORT INDICE **************************/



/************************** INSERTION SORT ************************************/

int Insertion_Sort(double *Entree, int nombre_d_elements, char sens) ;

/************************ FIN DE INSERTION SORT *******************************/


/************************** SELECTEUR DE RANG *********************************/

int R_Partitionner(double *Entree, double reference, int nombre_d_elements ) ;

double Rang_Select(double *Entree, int rang, int nombre_d_elements) ;

/***********************FIN DE SELECTEUR DE RANG ******************************/


/******************************* INSERSION  ***********************************/

int Insere(double *Tableau_deja_trie, double nouvel_element, int nombre_d_element_deja_trie) ;

/************************* FIN DE L'INSERSION ********************************/

/***************************** BUBBLE SORT ************************************/

int Bubble_Sort(double *Entree, int nombre_d_elements, char sens) ;

/*************************** FIN DE BUBBLE SORT *******************************/

#endif