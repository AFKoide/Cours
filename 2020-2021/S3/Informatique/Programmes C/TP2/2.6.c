#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float a;
    float b;
    float c;
    float d;
} matrice;

int somme_1(matrice gauche, matrice droite)
{
    matrice R;
    R.a = gauche.a + droite.a ;
    R.b = gauche.b + droite.b ;
    R.c = gauche.c + droite.c ;
    R.d = gauche.d + droite.d ;

    return R;
}

void somme_2(matrice gauche , matrice droite ,matrice* resultat )
{
    (*resultat).a = gauche.a + droite.a;
    (*resultat).b = gauche.b + droite.b;
    (*resultat).c = gauche.c + droite.c;
    (*resultat).d = gauche.d + droite.d;
}

void somme_3(matrice* gauche ,matrice* droite ,matrice* resultat)
{
    (*resultat).a = (*gauche).a + (*droite).a;
    (*resultat).b = (*gauche).b + (*droite).b;
    (*resultat).c = (*gauche).c + (*droite).c;
    (*resultat).d = (*gauche).d + (*droite).d;
}


{
    matrice M1, M2, M3;

    M3 = somme_1(M1, M2);


}