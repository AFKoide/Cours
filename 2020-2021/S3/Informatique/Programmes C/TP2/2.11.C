#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float a;
    float b;
    float c;
    float d;
} matrice;



matrice somme1(matrice gauche , matrice droite);
{
    matrice R ;

    R.a = (*gauche).a + (*droite).a;
    R.b = (*gauche).b + (*droite).b;
    R.c = (*gauche).c + (*droite).c;
    R.d = (*gauche).d + (*droite).d;

    return R;
}

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

matrice produit1(matrice gauche, matrice droite)
{
    matrice R;

    R.a = gauche.a * droite.a + gauche.b * droite.c;
    R.b = gauche.a * droite.b + gauche.b * droite.d;
    R.c = gauche.c * droite.a + gauche.d * droite.c;
    R.d = gauche.c * droite.b + gauche.d * droite.d;

    return R;
}


void main()
{
    int n;
    matrice Mfinal;

    printf("Nombre de matrice : ");
    scanf("%d",&n);

    matrice* T = malloc(10*sizeof(matrice));

    for (int i = 0; i < n; i++)
    {
        T[i] = saisie_matrice_1(void)
    }

    matrice R = T[0];
    for (i = 0; i < count; i++)
    {
        R = produit1(R,T[i]);
    }
    
}

////////////////////////////////////////////////////////////////
void saisie2(matrice* M)
{
    printf("valeur de a ? ");
    scanf("%f",&((*M).a) );
    printf("valeur de b ? ");
    scanf("%f",&((*M).b) );
    printf("valeur de c ? ");
    scanf("%f",&((*M).c) );
    printf("valeur de d ? ");
    scanf("%f",&((*M).d) );
}

void produit2(matrice* gauche, matrice* droite,matrice* resultat)
{
    (*resultat).a = (*gauche).a * (*droite).a + (*gauche).b * (*droite).c;
    (*resultat).b = (*gauche).a * (*droite).b + (*gauche).b * (*droite).d;
    (*resultat).c = (*gauche).c * (*droite).a + (*gauche).d * (*droite).c;
    (*resultat).d = (*gauche).c * (*droite).b + (*gauche).d * (*droite).d;
}

void main()
{
    int n;
    matrice Mfinal;

    printf("Nombre de matrice : ");
    scanf("%d",&n);

    matrice* T = malloc(10*sizeof(matrice));

    for (int i = 0; i < n; i++)
    {
        saisie2(&T[i]);
    }

    matrice R = T[0];
    for (i = 0; i < count; i++)
    {
        produit2(&R,&T[i],&S);
        R = S;
    }
    
}