#include <stdlib.h>
#include <stdio.h>

struct maillon0 {
    float valeur;
    struct maillon0* suivant;
    struct maillon0* precedent;
};
typedef struct maillon0 maillon;

int affichage(maillon* zero)
{
    maillon* m;

    printf("\nAffichage de la liste : \n");

    for(m = (*zero).suivant; m!=NULL;m = (*m).suivant)
    {
        printf("%f\t",(*m).valeur);
    }
    return 0;
}


maillon* insertion(maillon* chaine, float valeur)
{
    maillon* p;
    maillon* nouveau = malloc(1*sizeof(maillon));

    p = (*chaine).suivant;

    (*nouveau).valeur = valeur;
    (*nouveau).suivant = p;
    (*nouveau).precedent = chaine;
    (*chaine).suivant = nouveau;

    if(p != NULL)
    {
        (*p).precedent = nouveau;
    }

    return nouveau;
}


void main()
{
    maillon zero;
    zero.suivant = NULL;
    zero.precedent = NULL;

    maillon* p = &zero;

    p = insertion(p,2);
    p = insertion(p,3);
    p = insertion(p,-1);
    p = insertion(p,0);
    p = insertion(p,5);


    affichage(&zero);
}
