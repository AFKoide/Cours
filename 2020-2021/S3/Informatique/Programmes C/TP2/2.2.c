#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float a;
    float b;
    float c;
    float d;
} matrice;


int saisie_matrice_1(void)
{
    matrice M;

    printf("a : ");
    scanf("%f",&(M.a) );

    printf("b : ");
    scanf("%f",&(M.b) );

    printf("c : ");
    scanf("%f",&(M.c) );

    printf("d : ");
    scanf("%f",&(M.d) );

    return M;
}

void saisie_matrice_2(matrice* resultat)
{
    printf("a : ");
    scanf("%f",&((*M).a) );

    printf("b : ");
    scanf("%f",&((*M).b) );

    printf("c : ");
    scanf("%f",&((*M).c) );

    printf("d : ");
    scanf("%f",&((*M).d));
}



int main()
{
    matrice M1, M2;

    M1 = saisie_matrice_1();

    saisie_matrice_2(&M2);
}