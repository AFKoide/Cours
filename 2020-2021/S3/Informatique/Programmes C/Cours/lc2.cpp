#include <stdio.h>
#include <stdlib.h>

struct maillon {
int valeur;
maillon* suivant;
};

typedef struct maillon maillon;

maillon* insertion(maillon* m,int v)
{
maillon* nouveau;
nouveau = (maillon*)malloc(1*sizeof(maillon));
(*nouveau).valeur = v;
(*nouveau).suivant = (*m).suivant;
(*m).suivant = nouveau;

return nouveau;
}

int main()
{
maillon zero;
maillon* p;

zero.suivant = NULL;

p = &zero;
p=insertion(p,1);
p=insertion(p,2);
p=insertion(p,5);
p=insertion(p,8);
p=insertion(p,-4);
p=insertion(p,3);
p=insertion(p,7);

// affichage
maillon* i;
i = zero.suivant;
while(i!=NULL)
{
  printf("%d ",(*i).valeur);
  i = (*i).suivant;
}
printf("\n");

}
