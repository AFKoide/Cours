#include <stdio.h>
#include <stdlib.h>
#define MaxC 100


struct monome
{				//struct 1: contient coeficient et puissance

  int coef;
  int degre;

};

typedef struct monome Monome;

struct polynome
{				//struct 2: un tableau de struct 1 + sa taille
  Monome monome[MaxC];
  int nombreMonome;
};




typedef struct polynome Polynome;
Polynome remplissagePolynome ();
void affiche (Polynome p);
Polynome Tri_Poly (Polynome p);


main ()
{
  int i, n;
  Polynome p, q, r;
  p = remplissagePolynome ();
  puts ("\navant le tri:");
  affiche (p);
  printf ("\n");
  p = Tri_Poly (p);
  puts ("apres le tri:");
  affiche (p);



  printf ("\nfin");


}

//procedure d'affichage du polynome
/*
cette procedure assure l'affichage en  prenons en considC)ration les coeficient positif ou negatifs
elle assure un parfait affichage
*/
void
affiche (Polynome p)
{
  int i;

  printf ("\nP(x)= \t");
  printf ("%dX^%d", p.monome[0].coef, p.monome[0].degre);
  for (i = 1; i < p.nombreMonome; i++)
    {
      if (p.monome[i].coef >= 0)
	{
	  printf ("+%dX^%d", p.monome[i].coef, p.monome[i].degre);
	}
      else
	printf ("%dX^%d", p.monome[i].coef, p.monome[i].degre);
    }



}

//procedure de remplissage
/*
cette fonction de type polynome remplit le tableau
a partir du clavier et elle test les cas ou:
puissance =0 ou coefficient =0
*/
Polynome
remplissagePolynome ()
{
  Polynome p;
  int i, tmp_Deg, tmp_Coef;
  printf ("entrez le nombre des valeurs de votre Polynome :");
  scanf ("%d", &p.nombreMonome);

  for (i = 0; i < p.nombreMonome; i++)
    {
      tmp_Deg = 0;
      tmp_Coef = 0;
      printf ("entrez la coeficient de la valeur Num %d:", i + 1);
      scanf ("%d", &tmp_Coef);
      printf ("entrez la puissance de la valeur Num %d:", i + 1);
      scanf ("%d", &tmp_Deg);
      if (tmp_Coef == 0)
	{
	  p.monome[i].coef = 0;
	  p.monome[i].degre = 0;
	}
      else
	{
	  p.monome[i].coef = tmp_Coef;
	  p.monome[i].degre = tmp_Deg;
	}


    }
  return p;
}

//fonction de tri
Polynome
Tri_Poly (Polynome p)
{
  int taille = p.nombreMonome;
  int tab_en_ordre = 1, i;
  while (tab_en_ordre != 0)
    {
      tab_en_ordre = 0;
      for (i = 0; i < taille - 1; i++)
	{
	  if (p.monome[i].degre != p.monome[i + 1].degre)
	    {			//les puissances doivent etre differentes pour faire le tri sinon en passe au 2eme test
	      if (p.monome[i].degre < p.monome[i + 1].degre)
		{
		  Monome tmp;
		  tmp.coef = p.monome[i].coef;
		  tmp.degre = p.monome[i].degre;
		  p.monome[i].coef = p.monome[i + 1].coef;
		  p.monome[i].degre = p.monome[i + 1].degre;
		  p.monome[i + 1].coef = tmp.coef;
		  p.monome[i + 1].degre = tmp.degre;
		  tab_en_ordre = 1;
		}
	    }
	  else if (p.monome[i].coef < p.monome[i + 1].coef)
	    {			// ce test pour la puissance =0, dans ce cas la en fait le tri sur les coefficient puisque x puissance 0 =1.
	      Monome tmp;
	      tmp.coef = p.monome[i].coef;
	      tmp.degre = p.monome[i].degre;
	      p.monome[i].coef = p.monome[i + 1].coef;
	      p.monome[i].degre = p.monome[i + 1].degre;
	      p.monome[i + 1].coef = tmp.coef;
	      p.monome[i + 1].degre = tmp.degre;
	      tab_en_ordre = 1;
	    }
	}
      taille--;
    }

  return p;
}
