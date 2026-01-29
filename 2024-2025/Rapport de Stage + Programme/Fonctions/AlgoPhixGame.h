//
//  AlgoPhixGame.h
//  
//
//  Created by Olivier Strauss on 22/05/2025.
//

#ifndef AlgoPhixGame_h
#define AlgoPhixGame_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include "Tri.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

int CalculPhiX_game(double *x, double *X, int n, int k) ;
int CalculPhiX_MacSum(double *phi, double *x, double *X, int n) ;
int CalculPhiX(double *phi, double *x, double *X, int n, int k, char methode) ;
int CalculPhiX_linear(double *x, double *X, int n) ;
long long CalcuTailleGame(int n, int k);

# define LINEAR 0
# define GAME   1
# define MACSUM 2
# define MACSUM_INTERVALLE 3


#endif /* AlgoPhixGame_h */
