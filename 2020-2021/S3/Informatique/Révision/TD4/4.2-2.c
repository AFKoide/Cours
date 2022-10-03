#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void main_remplir()
{
    char nom[20];
    int note1, note2, note3;

    FILE * note;
    note = fopen("notes-2.txt","w");

    if(note != NULL)
    {
        for(int i=0; i<2; i++)
        {
            printf("Nom : ");
            scanf("%s",nom);

            printf("\nFrancais : ");
            scanf("%d",&note1);
            printf("\nAnglais : ");
            scanf("%d",&note2);
            printf("\nMath : ");
            scanf("%d",&note3);


            fprintf(note,"%d.\t %s : Francais %d, Anglais : %d, Math %d.\n",i,nom,note1,note2,note3);
        }
    }
    fclose(note);
}

void main_premier()
{
    char nom[20];
    int note1, note2, note3;

    FILE * note;
    note = fopen("notes-2.txt","w");

    if(note != NULL)
    {
        fread(nom,20,sizeof(char),note);
        fread(&note1,1,sizeof(int),note);
        fread(&note2,1,sizeof(int),note);
        fread(&note3,1,sizeof(int),note);

        printf("%s : Francais %d, Anglais : %d, Math %d.\n",i,nom,note1,note2,note3);
    }
    fclose(note);
}


void main_recherche()
{
    char nom[20];
    int note1, note2, note3;
    int curseur, id;

    FILE * note;
    note = fopen("notes-2.txt","w");

    id = 2;

    if(note != NULL)
    {
        curseur = (id-1)*(20*sizeof(char)+3*sizeof(int));
        fseek(note,curseur,SEEK_SET);

        fread(nom, 20, sizeof(char), f);
        fread( & note1, 1, sizeof(int), f);
        fread( & note2, 1, sizeof(int), f);
        fread( & note3, 1, sizeof(int), f);

        printf("%s a %d %d %d\n",nom,note1,note2,note3);

        fclose(f);
    }
}
