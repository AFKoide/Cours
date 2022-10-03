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

int produit_matrice(matrice* gauche, matrice* droite)
{
void produit2(matrice22* gauche, matrice22* droite,matrice22* resultat)
{
    (*resultat).a = (*gauche).a * (*droite).a + (*gauche).b * (*droite).c;
    (*resultat).b = (*gauche).a * (*droite).b + (*gauche).b * (*droite).d;
    (*resultat).c = (*gauche).c * (*droite).a + (*gauche).d * (*droite).c;
    (*resultat).d = (*gauche).c * (*droite).b + (*gauche).d * (*droite).d;
}
}

int somme_matrice(matrice* gauche, matrice* droite)
{
    matrice N ;

    N.a = (*gauche).a + (*droite).a;
    N.b = (*gauche).b + (*droite).b;
    N.c = (*gauche).c + (*droite).c;
    N.d = (*gauche).d + (*droite).d;

    return N;
}





void affichage(matrice* M)
{
    printf("%f",(*M).a);
    printf("%f",(*M).a);
    printf("%f",(*M).a);
    printf("%f",(*M).a);
}

void main()
{
    int M1, M2, M3, Mfinal;

    M1 = saisie_matrice();
    M2 = saisie_matrice();
    M3 = saisie_matrice();

    Mfinal = produit_matrice(&M2, (somme_matrice(&M1, &M3)))

    affichage(&Mfinal);
}