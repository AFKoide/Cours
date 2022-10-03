#include <stdlib.h>
#include <stdio.h>
struct maillon0
{
float valeur;
struct maillon0* suivant;
} ;

typedef struct maillon0 maillon;

maillon* insertion(maillon* M,float val)
{
    maillon* nouveau;
    nouveau = malloc(sizeof(maillon));
    (*nouveau).valeur = val;  // nouveau->valeur = val;
    (*nouveau).suivant = (*M).suivant;
    (*M).suivant = nouveau;
    return nouveau;
}

void afficheListe(maillon* zero)
{
  maillon* m;

  m = (*zero).suivant;
  printf("\nAffichage de la liste : \n");
  while(m!=NULL)
  {
    printf("%f\t",(*m).valeur);
    m = (*m).suivant;
  }
}


void afficheListe1(maillon* zero)
{
  maillon* m;

  printf("\nAffichage de la liste : \n");
  // for (i=0;i<10;i=i+1)
  for(m = (*zero).suivant; m!=NULL;m = (*m).suivant)
  {
    printf("%f\t",(*m).valeur);
  }
}

void suppression(maillon* precedent)
{
  maillon* a_supprimer;
  a_supprimer = (*precedent).suivant;
  (*precedent).suivant = (*a_supprimer).suivant;
  free(a_supprimer);
}

void ajout(float valeur, maillon* zero)
{
    for (m = (*zero); (*m).suivant != NULL; m = (*m.suivant))
    {
    }
    insertion(m, valeur);
}

void tableau(maillon* zero)
{

    int taille = taille_liste(zero);

    float* T = malloc(taille*sizeof(maillon));

    for (m = (*zero).suivant; m != NULL; i++)
    {
        T[i] = (*m).valeur;
        m = (*m).suivant;
    }


    Table[i] =
}

int taille_liste(maillon* zero)
{
    int i;
    maillon* m;
    for (m = (*zero).suivant; m != NULL; m = (*m).suivant)
    {
        i++;
    }
    return i;
}

int main()
{
maillon zero;
zero.suivant = NULL;
maillon* p = &zero;

p = insertion(p,2);
p = insertion(p,3);
p = insertion(p,-1);
p = insertion(p,0);
p = insertion(p,5);

afficheListe1(&zero);

suppression(&zero);
afficheListe1(&zero);

/*
int taille = taille_liste(&zero);
float* tableau = tableau(&zero);

for (int i = 0; i < taille; i++)
{
    printf("tableau[%d] = %f\n",i,tableau[i])
}

printf("%")
*/
return 0;
}
