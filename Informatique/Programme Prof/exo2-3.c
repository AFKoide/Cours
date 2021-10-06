#include <stdio.h>

typedef struct{
char nom[20];
char prenom[20];
long age;
int actif;

} contact;


contact saisie() {
contact C;
printf("Saisie d'un contact : \n");
printf("nom ? ");
scanf("%s",C.nom);
printf("prenom ? ");
scanf("%s",C.prenom);
printf("age ? ");
scanf("%ld",&C.age);
C.actif = 1;

return C;
}

int ajout(contact C,contact* Liste)
{
  int i=0;
  while(i<10 && Liste[i].actif == 1)
  {i=i+1;
  }

//  printf("Position : %d\n",i);
  if (i>=10)
  {
    return -1; // erreur : le carnet est plein
  }
  else{
    Liste[i] = C;
    Liste[i].actif = 1;
    }

    return 0; // tout s'est bien passé.

}

void affiche(contact C)
{
  printf("%s %s %ld\n",C.nom,C.prenom,C.age);

}

void afficheListe(contact* Liste)
{int i;
  printf("\nListe : \n");
  for(i=0;i<10;i=i+1)
  {
    if (Liste[i].actif == 1)
    {affiche(Liste[i]);
    }
    else{
      printf("(vide)\n");
    }
  }

}

void supprimer(contact* Liste,int position)
{
Liste[position].actif = 0;

}

int main()
{
  contact T[10];
  int i;
  // initialisation : aucun contact actif
  for(i=0;i<10;i=i+1)
  {
    T[i].actif = 0;
  }


int choix;
do {

printf("Menu :\n");
printf("0 - quitter le programme\n");
printf("1 - ajouter un contact\n");
printf("2 - supprimer un contact\n");
printf("3 - afficher tous les contacts\n");
printf("votre choix :  ");
scanf("%d",&choix);

switch(choix){
  case 1 :
    {
      contact C;
      C = saisie();
      ajout(C,T);
    }
    break;
  case 2 :
    {printf("\nQuel contact faut-il supprimer ?  (par numéro 1-10) : ");
     int numero;
     scanf("%d",&numero);
     numero = numero - 1 ;
     supprimer(T,numero);
    }
    break;
  case 3 :
   {
     afficheListe(T);
   }
   break;

}


} while(choix !=0);






  return 0;
}
