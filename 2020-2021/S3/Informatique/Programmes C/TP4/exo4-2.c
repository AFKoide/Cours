#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main421() {
  char nom[20];
  int note1, note2, note3;

  FILE * f;
  f = fopen("notes.dat", "w");
  if (f != NULL) {
    int i;
    for (i = 0; i < 5; i = i + 1) {
      printf("nom ? : ");
      scanf("%s", nom);
      printf("note 1 ? : ");
      scanf("%d", & note1);
      printf("note 2 ? : ");
      scanf("%d", & note2);
      printf("note 3 ? : ");
      scanf("%d", & note3);

      fwrite(nom, 20, sizeof(char), f);
      fwrite( & note1, 1, sizeof(int), f);
      fwrite( & note2, 1, sizeof(int), f);
      fwrite( & note3, 1, sizeof(int), f);

      fclose(f);
    }

  } else {
    printf("le fichier n'a pas pu etre ouvert !\n");
  }

  return 0;
}




int main422() {
  char nom[20];
  int note1, note2, note3;

  FILE * f;
  f = fopen("notes.dat", "r");
  if (f != NULL)
  {
    fread(nom, 20, sizeof(char), f);
    fread( & note1, 1, sizeof(int), f);
    fread( & note2, 1, sizeof(int), f);
    fread( & note3, 1, sizeof(int), f);

    printf("%s a %d %d %d\n", nom, note1, note2, note3);

    fclose(f);
  } else {
    printf("le fichier n'a pas pu etre ouvert !\n");
  }
  return 0;
}


int main423()
{
  char nom[20];
  int note1,note2,note3;

  int etudiant;
  int curseur;

  FILE* f;
  f = fopen("notes.dat","r");
  if(f!=NULL)
    {
      printf("Quel étudiant ?\n");
      scanf("%d", &etudiant);

      fseek(f,(etudiant-1) * (20*sizeof(char) + 3*sizeof(int), SEEK_SET);

      fseek(f) = curseur;

      fread(nom, 20, sizeof(char), f);
      fread( & note1, 1, sizeof(int), f);
      fread( & note2, 1, sizeof(int), f);
      fread( & note3, 1, sizeof(int), f);

      printf("%s a %d %d %d\n",nom,note1,note2,note3);

      fclose(f);
    }
  else
    {
    printf("le fichier n'a pas pu etre ouvert !\n");
    }

  return 0;

}


int main424()
{
  char nom[20];
  int note1,note2,note3;

  char etudiant[20];
  int curseur, i=0;;

  FILE* f;
  f = fopen("notes.dat","r");
  if(f!=NULL)
  {
    printf("Nom de l'étudiant à chercher ?    ");
    scanf("%s",etudiant);

    {
      fseek(f,i * (20*sizeof(char) + 3*sizeof(int), SEEK_SET);
      fread(nom,20,sizeof(char),f);

      if (strcmp(nom, etudiant)==0)
      {
        fread( & note1, 1, sizeof(int), f);
        fread( & note2, 1, sizeof(int), f);
        fread( & note3, 1, sizeof(int), f);
      }
      i++;
    }
    while (i<5)

  }



  return 0;
}
