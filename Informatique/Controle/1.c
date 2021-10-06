#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct termePoly {
    float coefficient;
    int exposant;
};
typedef struct termePoly termePoly;

struct Polynome {
    termePoly T[500];
    int nb_polynome;
};
typedef struct Polynome Polynome;


Polynome CreerPoly(int nbTermes)
 {
    polynome p;
    p.nb_polynome = nbTermes;

    for (int i = 0; i <nbTermes; i++)
    {
        p.T[i].exposant = -1;
    }
    return p;
}

Polynome AjoutTerme1(float coefficient, int exposant, Polynome P)
{
    for(int i = 0, i<P.nb_polynome; i++)
    {
        P.T[i].coefficient = coefficient;
        P.T[i].exposant = exposant;
    }
    return P;
}

void AjoutTerme2()


void main()
{
    float* T = malloc(500*sizeof(float));

    Polynome p;
    int nb_Poly = 20, coefficient, exposant;

    p = CreerPoly(nb_Poly);


    coefficient = 4;
    exposant = 1;

    p = Polynome AjoutTerme1(coefficient, exposant, p);
}
