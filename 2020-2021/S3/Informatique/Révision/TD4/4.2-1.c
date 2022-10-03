#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void main()
{
    char nom[20];
    int note1, note2, note3;

    FILE * note;
    note = fopen("notes.txt","w");

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


            fwrite(nom,20,sizeof(char),note);
            fwrite(&note1,1,sizeof(int),note);
            fwrite(&note2,1,sizeof(int),note);
            fwrite(&note3,1,sizeof(int),note);
        }
    }
    fclose(note);
}
