#include <stdlib.h>
#include <stdio.h>
struct maillon0
{
float valeur;
struct maillon0* suivant;
struct maillon0* precedent;

} ;

typedef struct maillon0 maillon;

maillon* insertion(maillon* M, float val)
{
maillon* nouveau = malloc(1*sizeof(maillon));
maillon* P;
P = (*M).suivant;
(*nouveau).valeur = val;
(*nouveau).suivant = P;
(*M).suivant = nouveau;
if(P!=NULL)
{(*P).precedent = nouveau;
}
(*nouveau).precedent = M;

return nouveau;
}


int main()
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

while((*p).precedent != NULL)
  {
    printf("%f\t",(*p).valeur);
    p = (*p).precedent;
  }
return 0;
}
