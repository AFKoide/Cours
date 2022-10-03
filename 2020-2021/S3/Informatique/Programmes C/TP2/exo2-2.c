#include <stdio.h>
#include <stdlib.h>

typedef struct {
float a,b,c,d;
} matrice22;


matrice22 saisie1()
{
matrice22 M;
printf("valeur de a ? ");
scanf("%f",&(M.a) );
printf("valeur de b ? ");
scanf("%f",&(M.b) );
printf("valeur de c ? ");
scanf("%f",&(M.c) );
printf("valeur de d ? ");
scanf("%f",&(M.d) );

return M;
}

void saisie2(matrice22* M)
{
  printf("valeur de a ? ");
  scanf("%f",&((*M).a) );
  printf("valeur de b ? ");
  scanf("%f",&((*M).b) );
  printf("valeur de c ? ");
  scanf("%f",&((*M).c) );
  printf("valeur de d ? ");
  scanf("%f",&((*M).d) );

}

void affichage1(matrice22 M)
{
printf("Affichage : \n");
printf("%f\t%f\n%f\t%f\n\n",M.a,M.b,M.c,M.d);

}

void affichage2(matrice22* M)
{
printf("Affichage : \n");
printf("%f\t%f\n%f\t%f\n\n",(*M).a,(*M).b,(*M).c,(*M).d);
}

matrice22 somme1(matrice22 gauche, matrice22 droite)
{
matrice22 R;
R.a = gauche.a + droite.a;
R.b = gauche.b + droite.b;
R.c = gauche.c + droite.c;
R.d = gauche.d + droite.d;

return R;
}

void somme2(matrice22 gauche, matrice22 droite,matrice22* resultat)
{
(*resultat).a = gauche.a + droite.a;
(*resultat).b = gauche.b + droite.b;
(*resultat).c = gauche.c + droite.c;
(*resultat).d = gauche.d + droite.d;
}

void somme3(matrice22* gauche, matrice22* droite,matrice22* resultat)
{
(*resultat).a = (*gauche).a + (*droite).a;
(*resultat).b = (*gauche).b + (*droite).b;
(*resultat).c = (*gauche).c + (*droite).c;
(*resultat).d = (*gauche).d + (*droite).d;
}

matrice22 produit1(matrice22 gauche, matrice22 droite)
{
matrice22 R;
R.a = gauche.a * droite.a + gauche.b * droite.c;
R.b = gauche.a * droite.b + gauche.b * droite.d;
R.c = gauche.c * droite.a + gauche.d * droite.c;
R.d = gauche.c * droite.b + gauche.d * droite.d;

return R;
}

void produit2(matrice22* gauche, matrice22* droite,matrice22* resultat)
{
  (*resultat).a = (*gauche).a * (*droite).a + (*gauche).b * (*droite).c;
  (*resultat).b = (*gauche).a * (*droite).b + (*gauche).b * (*droite).d;
  (*resultat).c = (*gauche).c * (*droite).a + (*gauche).d * (*droite).c;
  (*resultat).d = (*gauche).c * (*droite).b + (*gauche).d * (*droite).d;
}

void produit2b(matrice22* gauche, matrice22* droite,matrice22* resultat)
{
  matrice22 R;
  R.a = (*gauche).a * (*droite).a + (*gauche).b * (*droite).c;
  R.b = (*gauche).a * (*droite).b + (*gauche).b * (*droite).d;
  R.c = (*gauche).c * (*droite).a + (*gauche).d * (*droite).c;
  R.d = (*gauche).c * (*droite).b + (*gauche).d * (*droite).d;
  (*resultat) = R;
}


int main2211a()
{
  int n,i;
  printf("combien ?");
  scanf("%d",&n);
  matrice22* T = malloc(n*sizeof(matrice22)) ;

  for(i=0;i<n;i=i+1)
  {
    T[i] = saisie1();
  }

  matrice22 R;
  R = T[0];
  for(i=1;i<n;i=i+1)
  {  R = produit1(R,T[i]);
  }

  affichage1(R);
return 0;
}


int main2211b()
{
  int n,i;
  printf("combien ?");
  scanf("%d",&n);
  matrice22* T = malloc(n*sizeof(matrice22)) ;

  for(i=0;i<n;i=i+1)
  {
    saisie2(&T[i]);  // ou saisie2(T + i );
  }

  matrice22 R,S;
  R = T[0];
  for(i=1;i<n;i=i+1)
  {  produit2(&R,&T[i],&S);
     R = S;
  }

  affichage2(&R);
return 0;
}



int main2210a(){
  matrice22 M1,M2,M3,R;
  M1 = saisie1();
  M2 = saisie1();
  M3 = saisie1();

  R=produit1(M1,somme1(M2,M3));
  affichage1(R);

  return 0;
}

int main2210b(){
  matrice22 M1,M2,M3,M4,R;
  saisie2(&M1);
  saisie2(&M2);
  saisie2(&M3);

  somme2(M2,M3,&M4);
  produit2(&M1,&M4,&R);

  affichage2(&R);

  return 0;
}



int main229()
{
  matrice22 M1,M2,M3;
  saisie2(&M1);
  saisie2(&M2);

  M3 = M1;
  printf("## produit1 : \n");
  affichage2(&M1);
  affichage2(&M2);
  M1 = produit1(M1,M2);
  affichage2(&M1);
  affichage2(&M2);


  M1 = M3;
  printf("## produit2 : \n");
  affichage2(&M1);
  affichage2(&M2);
  produit2(&M1,&M2,&M1);
  affichage2(&M1);
  affichage2(&M2);

return 0;
}


int main()
{
  //main_general();
  //main229();
  //main2210a();
  //main2210b();
  //main2211a();
  main2211b();
  return 0;
}


int main_general()
{
matrice22 M1,M2,M3;

saisie2(&M1);
saisie2(&M2);

//M3 = somme1(M1,M2);
//somme2(M1,M2,&M3);
//somme3(&M1,&M2,&M3);
//M3 = produit1(M1,M2);
produit2(&M1,&M2,&M3);

affichage2(&M1);
affichage2(&M2);
affichage2(&M3);

return 0;
}
/*

int saisie_entier1()
{
int i;
printf("valeur ? ");
scanf("%d",&i);
return i;
}

void saisie_entier2(int* i)
{
int k;
printf("valeur ? ");
scanf("%d",&k);
*i = k;
}

void saisie_entier3(int* i)
{
printf("valeur ? ");
scanf("%d",i);
}

int main_entier()
{
int k,j,l;
k = saisie_entier1();
saisie_entier2(&j);
saisie_entier3(&l);


printf("valeur : %d\n",k);
printf("valeur : %d\n",j);
printf("valeur : %d\n",l);

  return 0;
}*/
