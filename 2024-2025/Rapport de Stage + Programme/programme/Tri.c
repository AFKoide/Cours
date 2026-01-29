#include <math.h>
#include <stdio.h>
#include "Tri.h"


/**********************************************************/
/***************** Fonctions de tri ***********************/
/***** Olivier STRAUSS 1997 ** LIRMM **********************/
/**********************************************************/
/**** Toutes les fonctions sont surchargees pour **********/
/**** fonctionner sur des tableaux de double, int *********/
/**** et unsigned char ************************************/
/**********************************************************/
/**********************************************************/


/**********************************************************/
/* Inverse_sens inverse l'ordre de son tableau d'entree   */
/* par parcours de ce tableau par deux pointeurs inverses */
/**********************************************************/

void Inverse_sens(double *Entree, int nombre_d_elements)
{
 double *pti, *ptf ;
 double temp ;

 pti = Entree ;
 ptf = Entree + nombre_d_elements - 1 ;

 while(pti<ptf)
 {
  temp = (*pti) ; (*pti) = (*ptf) ; (*ptf) = temp ;
  pti++ ; ptf-- ;
 }
}


void Inverse_sensInt(int *Entree, int nombre_d_elements)
{
 int *pti, *ptf ;
 int temp ;

 pti = Entree ;
 ptf = Entree + nombre_d_elements - 1 ;

 while(pti<ptf)
 {
  temp = (*pti) ; (*pti) = (*ptf) ; (*ptf) = temp ;
  pti++ ; ptf-- ;
 }
}

/********************** FIN DE INVERSE SENS ***********************************/



/************************** QUICK SORT ****************************************/

/**********************************************************/
/* Partitionnement du tableau A entre A[initial] et       */
/* A[final] de telle sorte que tout element de A qui      */
/* serait inferieur (rsp. superieur)a A[initial] serait   */
/* place dans la partie inferieure (rsp. superieure) du   */
/* tableau. L'indice de l'element median de la fonction   */
/* (dernier element de la partie inferieure) est retourne */
/* par la fonction                                        */
/* La fonction Q_Partitionner est appelee par Quick_Sort  */
/**********************************************************/

int Q_Partitionner( double *A, int initial, int final )
{
  double reference ;
  double *pti, *ptf ;
  int oui = 1 ;

  pti = A + initial ;
  ptf = A + final ;
  reference = (*pti) ;
  pti-- ; ptf++ ;

  while(oui)
  {
    while( (*(--ptf)) > reference ) ;
    while( (*(++pti)) < reference ) ;
    if( pti < ptf )
    {
    double temp ;
    temp = (*ptf) ; (*ptf) = (*pti) ; (*pti) = temp ;
    }
    else return (int)(ptf-A) ;
  }
  return 0 ;
}

/*****************************************************/
/* Q_Tri_Rapide : fonction principale du quick sort  */
/* Le principe est de diviser le tableau en 2 et de  */
/* grouper dans une partie du tableau les elements   */
/* qui sont inferieurs au 1er element du tableau     */
/* puis de recommencer dans chaque partie du tableau */
/*****************************************************/

int Q_Tri_Rapide(double *A, int *initial, int *final)
{
  int milieu ;

  if( (*initial) < (*final) )
  {
    milieu = Q_Partitionner( A , (*initial), (*final) ) ;
    Q_Tri_Rapide( A, initial, &milieu )  ;
    milieu++ ;
    Q_Tri_Rapide( A, &milieu , final )  ;
  }
  else
  {
    return 1 ; /* c'est fini */
  }
  return 0 ;
}

/**********************************************/
/* Algorithme de tri rapide par division      */
/* Entree : tableau de donnees                */
/*          nombre d'elements du tableau      */
/*          sens du tri :                     */
/*                      '+' ascendant         */
/*                      '-' descendant        */
/* Sortie : le tableau trie                   */
/*          0 si erreur, 1 si succes          */
/**********************************************/

int Quick_Sort(double *Entree, int nombre_d_elements, char sens)
{
  int initial = 0 , final = nombre_d_elements-1 ;

  Q_Tri_Rapide( Entree, &initial, &final) ;

  if(sens=='-') Inverse_sens(Entree, nombre_d_elements) ;

  return 1 ;
}


/************************* FIN DE QUICK SORT **********************************/


/************************** QUICK SORT INDICE *********************************/

/**********************************************************/
/* Partitionnement du tableau d'Entree entre Entree[0] et */
/* Entree[nombre_d_element-1] de telle sorte que tout     */
/* element de Entree qui serait inferieur (rsp. superieur)*/
/* a Entree[0] serait place dans la partie inferieure     */
/* (rsp. superieure) du tableau. L'indice de l'element    */
/* median du tableau (dernier element de la partie        */
/* inferieure) est retourne par la fonction. Le tableau   */
/* d'indice est trie en meme  temps. La fonction          */
/* Q_Partitionner est appelee par Quick_Sort_Indice.      */
/**********************************************************/
int QI_Partitionner( double *Entree, int *Indice, int nombre_d_elements )
{
  int oui = 1 ;
  double *pti_E, *ptf_E, temp_E ;
  double reference ;
  int *pti_I, *ptf_I, temp_I ;


  reference = (*Entree) ;
  pti_E = Entree - 1 ;
  ptf_E = Entree + nombre_d_elements ;
  pti_I = Indice - 1 ;
  ptf_I = Indice + nombre_d_elements ;

  while(oui)
  {
    do { ptf_E-- ; ptf_I-- ; } while( (*(ptf_E)) > reference ) ;
    do { pti_E++ ; pti_I++ ; } while( (*(pti_E)) < reference ) ;

    if( pti_E < ptf_E )
    {
    temp_E = (*ptf_E) ; (*ptf_E) = (*pti_E) ; (*pti_E) = temp_E ;
    temp_I = (*ptf_I) ; (*ptf_I) = (*pti_I) ; (*pti_I) = temp_I ;
    }
    else return (int)(ptf_E-Entree+1) ;
  }
  return 0 ;
}

/*****************************************************/
/* QI_Tri_Rapide : fonction principale du quick sort */
/* indice. Le principe est de diviser le tableau en  */
/* deux parties et de grouper dans la partie         */
/* inferieure (rsp. superieure) du tableau les       */
/* elements qui sont inferieurs (rsp superieurs) au  */
/* 1er element du tableau ( puis de recommencer dans */
/*chaque partie du tableau ainsi formee.             */
/*****************************************************/

int QI_Tri_Rapide(double *A, int *I, int n)
{
  int milieu ;

  if( n>1 )
  {
    milieu = QI_Partitionner( A , I, n ) ;
    QI_Tri_Rapide( A, I, milieu )  ;
    QI_Tri_Rapide( A+milieu, I+milieu , n-milieu)  ;
    return 1 ;
  }
  else
  {
    return 1 ; /* c'est fini */
  }
}

/***********************************************/
/* Algorithme inverse du Quick_Sort_Indice     */
/* Trie un tableau a partir de ses indices     */
/* contenus dans le tableau Indice             */
/* Algorithme lineaire (plus rapide qu'un tri) */
/***********************************************/

int Un_Quick_Sort_Indice(double *Entree, int *Indice, int nombre_d_elements, char sens)
{
 int oui = 1, incr ;
 double *pt_E, *pointeur_E, temp_E ;
 int *pt_I, *pointeur_I, temp_I ;

 pointeur_I = Indice ; incr = 0 ;

 while(oui)
 {
  /* recherche du premier mal classe */
  while( ( (*(pointeur_I)) == incr ) && ( incr < nombre_d_elements ) )
  {
	  pointeur_I++ ; incr++ ;
  }

  oui = incr-nombre_d_elements ;

  if(oui)
  {
  	pointeur_E = Entree + incr ;
  	pointeur_I = Indice + incr ;

  	while( (*pointeur_I)!=incr )
  	{
  	 pt_E = Entree + (*pointeur_I) ;
  	 pt_I = Indice + (*pointeur_I) ;
  	 temp_E = (*pt_E) ; (*pt_E) = (*pointeur_E) ; (*pointeur_E) = temp_E ;
	   temp_I = (*pt_I) ; (*pt_I) = (*pointeur_I) ; (*pointeur_I) = temp_I ;
  	}
  }
 }

 if(sens=='-') Inverse_sens(Entree, nombre_d_elements) ;

 return 1 ;
}

/********************************************************************/
/* Algorithme de tri rapide par division permettant de retrouver    */
/* rapidement l'ordre initial des elements en annexant un tableau   */
/* d'indices (Indices) qui doit soit etre alloue auparavant soit    */
/* pointer sur NULL                                                 */
/* Il ne faut pas oublier de le desallouer a la fin du traitement   */
/* Entree : tableau de donnees a trier                              */
/*          pointeur sur le tableau d'indices                       */
/*          nombre d'elements du tableau a trier                    */
/*          sens du tri (+ croissant, - decroissant)                */
/* Sortie : le tableau trie                                         */
/*          NULL si erreur, pointeur sur Indice si succes           */
/********************************************************************/

int Quick_Sort_Indice(double *Entree, int *Indice, int nombre_d_elements, char sens)
{
 int i, *pt ;

 for(i=0, pt=Indice ; i<nombre_d_elements ; i++, pt++) (*pt) = i ;

 if(nombre_d_elements<2) return 1 ;

 QI_Tri_Rapide( Entree, Indice, nombre_d_elements) ;

 if(sens=='-')
 {
  double *pti_E, *ptf_E, temp_E ;
  int    *pti_I, *ptf_I, temp_I ;

  pti_E = Entree ;
  ptf_E = Entree + nombre_d_elements - 1 ;
  pti_I = Indice ;
  ptf_I = Indice + nombre_d_elements - 1 ;

  while(pti_E<ptf_E)
  {
   temp_E = (*pti_E) ; (*pti_E) = (*ptf_E) ; (*ptf_E) = temp_E ;
   temp_I = (*pti_I) ; (*pti_I) = (*ptf_I) ; (*ptf_I) = temp_I ;
   pti_E++ ; ptf_E-- ; pti_I++ ; ptf_I-- ;
  }
 }

 return 1 ;
}

/************************** FIN DE QUICK SORT INDICE **************************/



/************************** INSERTION SORT ************************************/

/*********************************************************************/
/* Tri par insertion aussi appele tri du joueur de carte             */
/* son principe consiste a prendre chaque element dans l'ordre du    */
/* tableau d'entree et de l'inserer en son rang. cet algorithme est  */
/* en O(n�) dans le pire cas                                         */
/* Entree : tableau de donnees                                       */
/*          nombre d'elements du tableau                             */
/* Sortie : le tableau trie                                          */
/*          0 si erreur, 1 si succes                                 */
/*********************************************************************/

int Insertion_Sort(double *Entree, int nombre_d_elements, char sens)
{
 int i ;
 double retenue, *pt1, *pt2 ;

 for(i=1 ; i<nombre_d_elements ; i++)
 {
  pt1 = Entree + i ;
  pt2 = pt1 - 1 ;
  retenue = (*pt1) ;

  while( (pt2>=Entree) && ( (*pt2)>retenue ) )
  {
	(*pt1--) = (*pt2--) ;
  }
  (*pt1) = retenue ;
 }

 if(sens=='-') Inverse_sens(Entree, nombre_d_elements) ;

 return(1) ;
}


/************************ FIN DE INSERTION SORT *******************************/


/************************** SELECTEUR DE RANG *********************************/

/***********************************************************************/
/* Partitionnement equivalent a celui qui Quick_Sort (Q_Partitionner)  */
/* (version pointeurs) a la difference pres que l'element de reference */
/* du tri est donne en entree                                          */
/* Entree : tableau de donnees                                         */
/*          element de reference pour le partitionnement               */
/*          nombre d'elements du tableau                               */
/* Sortie : le tableau partitionne                                     */
/*          le nombre d'element du tableau inferieur                   */
/*                    (elements inferieurs ou egaux a la reference)    */
/***********************************************************************/

int R_Partitionner(double *Entree, double reference, int nombre_d_elements )
{
 int oui = 1 ;
 double *pti, *ptf, temp ;

 pti = Entree - 1 ;
 ptf = Entree + nombre_d_elements ;

 while(oui)
 {
  do ptf-- ; while( (*(ptf)) > reference ) ;
  do pti++ ; while( (*(pti)) < reference ) ;
  if( pti < ptf )
  {
	temp = (*ptf) ; (*ptf) = (*pti) ; (*pti) = temp ;
  }
  else return (int)(ptf-Entree+1) ;
 }
 return 0;
}

/*********************************************************************/
/* Rang_Select selectionne l'element de rang 'rang' dans le tableau  */
/* d'Entree de nombre_d_elements elements. Ce rang serait le sien si */
/* le tableau etait trie mais Rang_Select n'effectue pas de tri      */
/* Le tableau d'entree effectue cependant un deplacement des indices */
/* des elements du tableau. Si on veut conserver ce tableau intact   */
/* est necessaire de le recopier                                     */
/* Entree : tableau de donnees                                       */
/*          rang de l'element a trouer                               */
/*          nombre d'elements du tableau                             */
/* Sortie : element de rang 'rang'                                   */
/*********************************************************************/

double Rang_Select(double *Entree, int rang, int nombre_d_elements)
{
 double *ptE, *ptR ;
 int N, reste ;
 int i ;
 double temp, mediane ;

 if(nombre_d_elements <=5 )
 {
  Insertion_Sort( Entree, nombre_d_elements, '+' ) ;
  return Entree[rang] ;
 }

 N = nombre_d_elements / 5 ;
 reste = nombre_d_elements % 5 ;

 /* Etapes 1 et 2 de l'algorithme : division du table d'entree en nb_pts/5 */
 /* tableau de 5 elements + un de nbpts mod 5 elements puis reorganisation */
 /* de ces sous-tableaux par insertion pour en obtenir la mediane          */

 for( i=0, ptE=Entree ; i<N ; i++, ptE+=5 )
 {
  Insertion_Sort( ptE, 5, '+' ) ;
 }

 if(reste) Insertion_Sort( ptE, reste, '+' ) ;

 /* On amene l'ensemble des medianes au debut du tableau */

 for( i=0, ptE=Entree, ptR=Entree+2 ; i<N ; i++, ptE++, ptR+=5 )
 {
  temp = (*ptR) ; (*ptR) = (*ptE) ; (*ptE) = temp ;
 }

 if(reste)
 {
  ptR = Entree + nombre_d_elements - (reste+1)/2 ;
  temp = (*ptR) ; (*ptR) = (*ptE) ; (*ptE) = temp ;
 }

 if(reste) N = N+1 ;

 reste = (N/2) ;  /* rang de la mediane */

 mediane = Rang_Select(Entree, reste, N) ;
 reste = R_Partitionner( Entree, mediane, nombre_d_elements ) ;

 /* Il y a (reste) elements inferieurs a la mediane */

 if( rang < reste )
 {
  mediane = Rang_Select( Entree, rang, reste ) ;
 }
 else
 {
  mediane = Rang_Select( Entree+reste , rang-reste , nombre_d_elements-reste ) ;
 }

 return mediane ;

}

/***********************FIN DE SELECTEUR DE RANG ******************************/


/******************************* INSERSION  ***********************************/

/**********************************************************/
/* Insere un element dans le Tableau deja trie            */
/* en le parcourant a partir de la fin jusqu'a            */
/* trouver la place du nouvel element                     */
/* Cette fonction est utile dans le cas ou l'on voudrait  */
/* effectuer un tri recursif                              */
/* On peut cependant toujours utiliser les fonctions      */
/* de tri ci-dessus                                       */
/**********************************************************/


int Insere(double *Tableau_deja_trie, double nouvel_element, int nombre_d_element_deja_trie)
{
 double *dernier, *avant_dernier ;

 if(nombre_d_element_deja_trie)
 {
  dernier = Tableau_deja_trie + nombre_d_element_deja_trie ;
  avant_dernier = dernier - 1 ;

  while( (avant_dernier>=Tableau_deja_trie) && ( (*avant_dernier)>nouvel_element ) )
  {
   (*dernier--) = (*avant_dernier--) ;
  }
  (*dernier) = nouvel_element ;
 }
 else
 {
  (*Tableau_deja_trie) = nouvel_element ;
 }

 return 1 ;
}

/************************* FIN DE L'INSERSION ********************************/


/***************************** BUBBLE SORT ************************************/

/**********************************************/
/* Algorithme de tri di "a bulle" : valable   */
/* dans le cas de "petits tableaux" (100 env) */
/* Entree : tableau de donnees                */
/*          nombre d'elements du tableau      */
/*          sens du tri :                     */
/*                      '+' ascendant         */
/*                      '-' descendant        */
/* Sortie : le tableau trie                   */
/*          0 si erreur, 1 si succes          */
/**********************************************/

int Bubble_Sort(double *Entree, int nombre_d_elements, char sens)
{
 int oui, i ;
 double *p,sol ;

 nombre_d_elements-- ;

 switch(sens)
 {
  case '+' : /* Sens ascendant : P[i] < P[i+1] */
  {
	do
	{
	 p = Entree ;
	 oui = 0 ;
	 for(i=0 ; i<nombre_d_elements ; i++, p++)
	 {
	  if( (*p) > (*(p+1)) )
	  {
		sol = (*p) ; (*p) = (*(p+1)) ; (*(p+1)) = sol ;
		oui = 1 ;
	  }
	 }
	}
	while(oui) ;
  } break ;

  case '-' : /* Sens descendant : P[i] > P[i+1] */
  {
	do
	{
	 p = Entree ;
	 oui = 0 ;
	 for(i=0 ; i<nombre_d_elements ; i++, p++)
	 {
	  if( (*p) < (*(p+1)) )
	  {
		sol = (*p) ; (*p) = (*(p+1)) ; (*(p+1)) = sol ;
		oui = 1 ;
	  }
	 }
	}
	while(oui) ;
  } break ;
 }
 return 1 ;
}

/*************************** FIN DE BUBBLE SORT *******************************/



/*********************************************
 * Main permettant de tester les algorithmes *
 * Rang_Select, Quick_Sort et Insertion_Sort *
 *********************************************/


/*

void main(int argc, char *argv[])
{
 int i, N ;
 time_t temps ;
 double *Avant, *Apres ;
 double mediane=0.0 ;
 int quick=0, *Indice ;
 FILE *fichier ;


 ctime(&temps) ;
 srand((int)temps) ;

 if(argc>1) N = atoi(argv[1]) ;
 else N = rand()%1000 ;
 if(argc>2) quick = (argv[2][0]=='q') || (argv[2][0]=='Q') ;

 quick = quick ;

 Avant = new double [N] ;
 Apres = new double [N] ;
 Indice = new int [N] ;

 if( (Avant==NULL) || (Apres==NULL) || (Indice==NULL) )
 {
  printf("\nPas assez de memoire") ; exit(0) ;
 }

 fichier = fopen(".\\result.txt","wt") ;
 if(fichier==NULL) exit(0) ;

 for(i=0 ; i<N ; i++) Avant[i] = (double)(rand()%1000) ;
 for(i=0 ; i<N ; i++) Apres[i] = Avant[i] ;


 Quick_Sort_Indice(Apres, Indice, N, '+') ;
 for(i=0 ; i<N ; i++)
 {
  fprintf(fichier,"\n(%3d) [%3.0lf]\t(%3d) [%3.0lf]",i,Avant[i],Indice[i],Apres[i]) ;
 }

 Un_Quick_Sort_Indice( Apres, Indice, N, '-' ) ;

 fprintf(fichier,"\n\nEt apres le de-tri\n") ;
 for(i=0 ; i<N ; i++)
 {
  fprintf(fichier,"\n(%3d) [%3.0lf] [%3.0lf]",i,Avant[N-i-1],Apres[i]) ;
  if(Avant[N-i-1]-Apres[i]) fprintf(fichier," ?") ;
  else                  fprintf(fichier," !") ;
 }

 for(i=0 ; i<N ; i++) Apres[i] = Avant[i] ;

 fprintf(fichier,"\n\nBubble sort\n") ;
 Insertion_Sort(Apres,N,'+') ;
 for(i=0 ; i<N ; i++)
 {
  fprintf(fichier,"\n(%3d) [%3.0lf] [%3.0lf]",i,Avant[i],Apres[i]) ;
  Apres[i] = Avant[i] ;
 }

 mediane = Rang_Select(Apres, N/2, N) ;
 fprintf(fichier,"\n\nmediane=%g\n",mediane) ;

 delete [] Avant ;
 delete [] Apres ;
 delete [] Indice ;

 fclose(fichier) ;
}

*/