//
//  AlgoPhixGame.c
//
//  Created by Olivier Strauss on 22/05/2025.
//
#include "AlgoPhixGame.h"

long long factorial(int n) {
  long long fact_n = 1, i;

  if (n < 0) return -1;
  for (i = 2; i <= n; i++) {
    fact_n *= i;
  }
  return fact_n;
}

long long combinaison(int n, int p) {
  long long c_n_p;

  if (n < 0 || p > n) return -1;
  c_n_p = factorial(n);
  c_n_p /= factorial(p) * factorial(n - p);
  return c_n_p;
}

long long CalcuTailleGame(int n, int k) {
  long long N = 0; 
  int p;

  for (p = 1; p <= k; p++) {
    N += combinaison(n, p);
  }
  return N;
}

int CalculPhiX_game(double *x, double *X, int n, int k) {
  static char *indice_max;
  static int init = 1;
  static long long N = 0;

  double *ptX, *ptx, *fin;
  int p, i, cpt;
  int q, q_debut, q_fin, q_repere;

  if (init) {
    init = 0;
    N = CalcuTailleGame(n, k);
    indice_max = (char *)malloc(N * sizeof(char));
    if (indice_max == NULL) {
      fprintf(stderr, "Memory allocation failed for indice_max.\n");
      init = 1;
      return EXIT_FAILURE;
    }
  }

  ptX = X;
  ptx = x;
  fin = ptx + n;
  while (ptx < fin) (*ptX++) = (*ptx++);
  for (i = 0; i < n; i++) indice_max[i] = i;

  cpt = n - 1;
  q_debut = 0;
  q_fin = n;

  for (p = 2; p <= k; p++) {
    q_repere = cpt + 1;
    for (q = q_debut; q < q_fin; q++) {
      for (i = indice_max[q] + 1; i < n; i++) {
        cpt++;
        X[cpt] = MIN(x[i], X[q]);
        indice_max[cpt] = i;
      }
    }
    q_debut = q_repere;
    q_fin = cpt;
  }
  return EXIT_SUCCESS;
}

int CalculPhiX_linear(double *x, double *X, int n) {
  double *ptX = X, *ptx = x, *fin = x + n;
  while (ptx < fin) (*ptX++) = (*ptx++);
  return EXIT_SUCCESS;
}


int CalculPhiX_MacSum(double *phi, double *x, double *X, int n) {
  int *indice, *pti, k;
  double *phi_sort, *pt, *tp, *fin;
  double x_min, x_max, x_cur;

  indice = (int *)malloc(n * sizeof(int));
  if (indice == NULL) 
    return EXIT_FAILURE;

  phi_sort = (double *)malloc(n * sizeof(double));
  if (phi_sort == NULL) {
    free(indice);
    return EXIT_FAILURE;
  }

  pt = phi_sort;
  tp = phi;
  fin = pt + n;
  pti = indice;
  k = 0;
  while (pt < fin) {
    (*pt++) = (*tp++);
    (*pti++) = k++;
  }

  pt = X;
  fin = pt + n;
  while (pt < fin) (*pt++) = 0.0;

  Quick_Sort_Indice(phi_sort, indice, n, '+');

  pti = indice;
  pt = phi_sort;
  fin = pt + n;
  x_min = x[*pti];
  x_max = x[*pti];
  if ((*pt) <= 0.0) {
    X[*pti] += x_min;
    X[*pti] += x_max;
  }
  pt++;
  pti++;
  while (pt < fin) {
    if ((*pt) <= 0.0) {
      X[*pti] -= x_min;
      X[*pti] -= x_max;
    }
    x_cur = x[*pti];
    x_min = MIN(x_min, x_cur);
    x_max = MAX(x_max, x_cur);
    if ((*pt) <= 0.0) {
      X[*pti] += x_min;
      X[*pti] += x_max;
    }
    pt++;
    pti++;
  }

  Inverse_sensInt(indice, n);
  Inverse_sens(phi_sort, n);

  pti = indice;
  pt = phi_sort;
  fin = pt + n;
  x_min = x[*pti];
  x_max = x[*pti];
  if ((*pt) >= 0.0) {
    X[*pti] += x_max;
    X[*pti] += x_min;
  }
  pt++;
  pti++;
  while (pt < fin) {
    if ((*pt) >= 0.0) {
      X[*pti] -= x_max;
      X[*pti] -= x_min;
    }
    x_cur = x[*pti];
    x_min = MIN(x_min, x_cur);
    x_max = MAX(x_max, x_cur);
    if ((*pt) >= 0.0) {
      X[*pti] += x_max;
      X[*pti] += x_min;
    }
    pt++;
    pti++;
  }
  free(phi_sort);
  free(indice);

  return EXIT_SUCCESS;
}

// Pour cet algorithme, X est de taille 2xn
int CalculPhiX_MacSum_Intervalle(double *phi, double *x, double *X, int n)
{
  int *indice, *pti, k;
  double *phi_sort, *pt, *tp, *fin;
  double x_min, x_max, x_cur;
  double *X_sup, *X_inf ;
    
  X_inf = X ;
  X_sup = X + n ;

  indice = (int *)malloc(n * sizeof(int));
  if (indice == NULL) return 0;

  phi_sort = (double *)malloc(n * sizeof(double));
  if (phi_sort == NULL) {
    free(indice);
    return EXIT_FAILURE;
  }

  pt = phi_sort;
  tp = phi;
  fin = pt + n;
  pti = indice;
  k = 0;
  while (pt < fin) {
    (*pt++) = (*tp++);
    (*pti++) = k++;
  }

  pt = X;
  fin = pt + 2*n;
  while (pt < fin) (*pt++) = 0.0;

  Quick_Sort_Indice(phi_sort, indice, n, '+');

  pti = indice;
  pt = phi_sort;
  fin = pt + n;
  x_min = x[*pti];
  x_max = x[*pti];
  if ((*pt) <= 0.0) {
    X_sup[*pti] += x_min;
    X_inf[*pti] += x_max;
  }
  pt++;
  pti++;
  while (pt < fin) {
    if ((*pt) <= 0.0) {
      X_sup[*pti] -= x_min;
      X_inf[*pti] -= x_max;
    }
    x_cur = x[*pti];
    x_min = MIN(x_min, x_cur);
    x_max = MAX(x_max, x_cur);
    if ((*pt) <= 0.0) {
      X_sup[*pti] += x_min;
      X_inf[*pti] += x_max;
    }
    pt++;
    pti++;
  }

  Inverse_sensInt(indice, n);
  Inverse_sens(phi_sort, n);

  pti = indice;
  pt = phi_sort;
  fin = pt + n;
  x_min = x[*pti];
  x_max = x[*pti];
  if ((*pt) >= 0.0) {
    X_sup[*pti] += x_max;
    X_inf[*pti] += x_min;
  }
  pt++;
  pti++;
  while (pt < fin) {
    if ((*pt) >= 0.0) {
      X_sup[*pti] -= x_max;
      X_inf[*pti] -= x_min;
    }
    x_cur = x[*pti];
    x_min = MIN(x_min, x_cur);
    x_max = MAX(x_max, x_cur);
    if ((*pt) >= 0.0) {
      X_sup[*pti] += x_max;
      X_inf[*pti] += x_min;
    }
    pt++;
    pti++;
  }
  free(phi_sort);
  free(indice);

  return EXIT_SUCCESS;
}


int CalculPhiX(double *phi, double *x, double *X, int n, int k, char methode) {
  int retour = EXIT_FAILURE;
  switch (methode) {
    case MACSUM:
      retour = CalculPhiX_MacSum(phi, x, X, n);
      break;
    case GAME:
      retour = CalculPhiX_game(x, X, n, k);
      break;
    case LINEAR:
      retour = CalculPhiX_linear(x, X, n);
      break;
    case MACSUM_INTERVALLE:
      retour = CalculPhiX_MacSum_Intervalle(phi, x, X, n);
      break;
  }
  return retour;
}
