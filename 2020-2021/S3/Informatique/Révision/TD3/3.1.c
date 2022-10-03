#include <stdlib.h>
#include <stdio.h>

struct maillon0 {
    float valeur;
    struct maillon0* suivant;
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

maillon* insertion(maillon* adresse,float valeur)
{
    maillon* ajouter;
    ajouter = malloc(sizeof(maillon));

    (*ajouter).valeur = valeur;
    (*ajouter).suivant = (*adresse).suivant;

    (*adresse).suivant = ajouter;

    return ajouter;
}


maillon* suppression(maillon* precedent)
{
    maillon* supprimer;
    supprimer = (*precedent).suivant;

    (*precedent).suivant = (*supprimer).suivant;

    free(supprimer);
}

maillon* ajout(int valeur, maillon* zero)
{
    maillon* m;
    for(m = &zero ; (*m).suivant != NULL ; m = (*m).suivant)

    insertion(m,valeur);
}

void tableau(maillon* zero)
{
    int taille = taille_liste(&zero);
    int i = 0;

    int* T = malloc(taille*sizeof(maillon));

    maillon* m;

    for((*m).suivant = (*zero).suivant ; (*m).suivant != NULL ; i++)
    {
        T[i] = (*m).valeur;
        (*m).suivant = (*m).suivant;
        printf(i);
    }
    affiche_tableau(&tableau,taille);
    printf("a");
}

int taille_liste(maillon* zero)
{
    maillon* m;
    int i=0;

    for(m = &zero ; (*m).suivant != NULL ; m = (*m).suivant)
    {
        i++;
    }

    return i;
}

void affiche_tableau(int* tableau, int taille)
{
    printf("\n\n\nTableau : \n");
    for(int i = 0 ; i<taille;i++)
        printf("%d : %d\n",taille, tableau);
}


void main()
{
    maillon zero;
    zero.suivant = NULL;

    maillon* p = &zero;

    p = insertion(p,8);
    p = insertion(p,2);
    p = insertion(p,7);
    p = insertion(p,4);
//    suppression(&zero);
    affichage(&zero);

//    ajout(14,&zero);

//    affichage(&zero);
    printf("a");
    tableau(&zero);

}
