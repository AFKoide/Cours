//
//  MacsumComputation.h
//  
//
//  Created by Olivier Strauss on 08/04/2025.
//

#ifndef MacsumComputation_h
#define MacsumComputation_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Tri.h"

#define Min(a,b) ( ( (a) < (b) ) ? (a) : (b) )
#define Max(a,b) ( ( (a) > (b) ) ? (a) : (b) )

int CalculMacsum(double *phi, double *x, double *phi_x_inf, double *phi_x_sup, int n) ;

#endif /* MacsumComputation_h */
