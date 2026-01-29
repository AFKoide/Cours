//
//  MacsumComputation.c
//  
//
//  Created by Olivier Strauss on 08/04/2025.
//

#include "MacsumComputation.h"


int CalculMacsum(double *phi, double *x, double *phi_x_inf, double *phi_x_sup, int n)
{
 int *indice, *pti, k ;
 double *phi_sort, *pt, *tp, *fin ;
 double x_min, x_max, x_cur ;
 
 indice = (int *)malloc( n * sizeof(int) ) ;
 if(indice==NULL) return 0 ;

 phi_sort = (double *)malloc( n * sizeof(double) ) ;
 if(phi_sort==NULL) { free(indice) ; return 0 ; }
 
 pt = phi_sort ; tp = phi ; fin = pt + n ; pti = indice ; k = 0 ;
 while(pt<fin) { (*pt++) = (*tp++) ; (*pti++) = k++ ; }
 pt = phi_x_inf ; tp = phi_x_sup ; fin = pt + n ;
 while(pt<fin) { (*pt++) = 0.0 ; (*tp++) = 0.0 ; }

 Quick_Sort_Indice(phi_sort, indice, n, '+') ; // tri de phi dans le sens croissant
 
 pti = indice ; pt = phi_sort ; fin = pt + n ;
 x_min = x[*pti] ; x_max = x[*pti] ;
 if( (*pt) <= 0.0 )
 {
  phi_x_sup[*pti] += x_min ;
  phi_x_inf[*pti] += x_max ;
 }
 pt++ ; pti++ ;
 while(pt<fin)
 {
  if( (*pt) <= 0.0 )
  {
   phi_x_sup[*pti] -= x_min ;
   phi_x_inf[*pti] -= x_max ;
  }
  x_cur = x[*pti] ;
  x_min = Min(x_min, x_cur) ;
  x_max = Max(x_max, x_cur) ;
  if( (*pt) <= 0.0 )
  {
   phi_x_sup[*pti] += x_min ;
   phi_x_inf[*pti] += x_max ;
  }
  pt++ ; pti++ ;
 }

 Inverse_sensInt(indice, n) ; // tri de phi dans le sens decroissant
 Inverse_sens(phi_sort, n) ;

 pti = indice ; pt = phi_sort ; fin = pt + n ;
 x_min = x[*pti] ; x_max = x[*pti] ;
 if( (*pt) >= 0.0 )
 {
  phi_x_sup[*pti] += x_max ;
  phi_x_inf[*pti] += x_min ;
 }
 pt++ ; pti++ ;
 while(pt<fin)
 {
  if( (*pt) >= 0.0 )
  {
   phi_x_sup[*pti] -= x_max ;
   phi_x_inf[*pti] -= x_min ;
  }
  x_cur = x[*pti] ;
  x_min = Min(x_min, x_cur) ;
  x_max = Max(x_max, x_cur) ;
  if( (*pt) >= 0.0 )
  {
   phi_x_sup[*pti] += x_max ;
   phi_x_inf[*pti] += x_min ;
  }
  pt++ ; pti++ ;
 }
 free(phi_sort) ; free(indice) ;
 return 1 ;
}

int MacSum(double *phi, double *x, double *y_inf, double *y_sup, int n)
{
 double *x_sort, *pt, *tp, *w ;
 int *pti, *indice, k ;
 double alpha, beta, gamma ;
 
 
 *y_sup=0.0 ; *y_inf=0.0 ;
 
 x_sort = (double *)malloc( n * sizeof(double) ) ;
 if(x_sort==NULL) return 0 ;
 indice = (int *)malloc( n * sizeof(int) ) ;
 if(indice==NULL) { free(x_sort) ; return 0 ; }
 
 // Estimation de *y_sup
 pti = indice ;
 tp = x ;
 pt = x_sort ;

 for( k=0 ; k<n ; k++, pt++, tp++, pti++ ) { (*pti) = k ; (*pt) = (*tp) ; }

 Quick_Sort_Indice(x_sort, indice, n, '-') ;

 pti = indice ;
 pt = x_sort ;
 alpha = 0.0 ;
 for( k=0 ; k<n ; k++ )
 {
  beta = alpha ;
  gamma = phi[*pti++] ;
  alpha = Max( alpha, gamma ) ;
  *y_sup += (alpha - beta) * (*pt++) ;
 }

 Inverse_sens(x_sort, n) ;
 Inverse_sensInt(indice, n) ;
 
 pti = indice ;
 pt = x_sort ;
 alpha = 0.0 ;
 for( k=0 ; k<n ; k++ )
 {
  beta = alpha ;
  gamma = phi[*pti++] ;
  alpha = Min( alpha, gamma ) ;
  *y_sup += (alpha - beta) * (*pt++) ;
 }

 // Estimation de *y_inf
  
 pti = indice ;
 pt = x_sort ;
 alpha = 0.0 ;
 for( k=0 ; k<n ; k++ )
 {
  beta = alpha ;
  gamma = phi[*pti++] ;
  alpha = Max( alpha, gamma ) ;
  *y_inf += (alpha - beta) * (*pt++) ;
 }

 Inverse_sens(x_sort, n) ;
 Inverse_sensInt(indice, n) ;

 pti = indice ;
 pt = x_sort ;
 alpha = 0.0 ;
 for( k=0 ; k<n ; k++ )
 {
  beta = alpha ;
  gamma = phi[*pti++] ;
  alpha = Min( alpha, gamma ) ;
  *y_inf += (alpha - beta) * (*pt++) ;
 }
 
 free(indice) ;
 free(x_sort) ;
 return 1 ;
}

int main(void)
{
 int i, n ;
 double x[10], phi[10], phi_x_inf[10], phi_x_sup[10] ;
 double y_inf, y_sup ;
 double z_inf, z_sup ;

 n=10 ;
 
 srand( time( NULL ) );
 
 for(i=0 ; i<n ; i++)
 {
  x[i] = (double)rand()/(double)RAND_MAX ;
  phi[i] = (double)rand()/(double)RAND_MAX ;
 }
 
 CalculMacsum(phi, x, phi_x_inf, phi_x_sup, n) ;
 z_inf = 0.0 ; z_sup = 0.0 ;
 for(i=0 ; i<n ; i++) { z_sup += phi[i] * phi_x_sup[i] ; z_inf += phi[i] * phi_x_inf[i] ; }
 MacSum(phi, x, &y_inf, &y_sup, n) ;
 
 printf("[ (%g, %g) (%g, %g) ]\n",y_inf, z_inf, y_sup, z_sup) ;
}
