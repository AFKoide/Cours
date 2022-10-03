#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float a;
    float b;
    float c;
    float d;
} matrice;

void affichage_1(matrice M)
{
    printf("%f",M.a);
    printf("%f",M.a);
    printf("%f",M.a);
    printf("%f",M.a);
}

void affichage_2(matrice* M)
{
    printf("%f",(*M).a);
    printf("%f",(*M).a);
    printf("%f",(*M).a);
    printf("%f",(*M).a);
}


main()
{
    matrice M1;
    affichage_1()
    
    affichage_2(&M1);
}