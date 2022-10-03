#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nom[20];
    char prenom[20];
    long age;
    int actif;  //On peut pas l'initialiser car le struct n'autorise pas cela.
} contact; 

contact saisie()
{
    contact Tempo;

    printf("Nom : ");
    scanf("%s",Tempo.nom);

    printf("Prenom : ");
    scanf("%s",Tempo.prenom);

    printf("Age : ");
    scanf("%l",,&Tempo.age);

    C.actif = 1;

    return Tempo
}

void ajout_contact(contact C, contact* Liste)
{
    int i;
    while (i<10 && liste[i].actif == 1)
    {
        i++;
    }
    
    if (i>=10)
    {
        return -1;
    }
    else
    {
        Liste[i] = C;
        Liste[i].actif = 1;
    }
    
    return 0;
}

void supprimer_contact(int numero ,contact* Liste)
{
    
    Liste[numero].actif = 0;
    
}

void affiche(contact C)
{
    printf("%s %s %l\n",C.nom, C.prenom, C.age);
}

void afficher_liste(contact* Liste)
{
    for (int i = 0; i < 10; i++)
    {
        if ((i<10 && liste[i].actif == 1))
        {
            affiche(Liste[i]);
        }
    }
    
}

int main()
{
    contact* T = malloc(10*sizeof(contact));
    for (int i = 0; i < 10; i++)
    {
        T[i].actif = 0;
    }
    int curseur, numero, quitter = 0;

    do
    {
    printf('%%% Carnet de contact : %%%'"\n");

    printf("Quel opération réaliser :\n 1 : ajout\n2 : suppression\n3 : liste\n0 : quitter");
    scanf("%d",&curseur);

    switch (curseur)
    {
        case 1:
        {
            contact C;
            C = saisie();
            ajout_contact(C, T);
        }
            break;

        case 2 :
        {
            printf("Contact à supprimer : ");
            scanf("%d",numero);
            numero = numero-1;
            supprimer_contact(C, T);
        }
        break;

        case 3 :
        {
            afficher_liste(T);
        }

        case 0 :
        quitter = 1;
        break;

        default:
        printf("Erreur");
            break;
    }
    }
    while(choix!=1)
    


    return 0;
}

