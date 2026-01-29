#ifndef MAIN_H
#define MAIN_H

// ------------------------

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "AlgoPhixGame.h"
#include "MacsumComputation.h"

// ------------------------

#define True 1
#define False 0

// ------------------------

typedef struct
{
    double y_sup;
    double y_inf;
} Y;    

typedef struct {
    double value;
    int index;
} Pair;

// ------------------------

#endif // MAIN_H