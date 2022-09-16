#include "grille.h"

pion ListePions[50];


void preparePions()
{
  int k;
  for(k=0;k<50;k++)
  {
    ListePions[k].actif = 0;
  }

int x,y;
k = 0;
for(y=0;y<4;y++)
{
  for(x=0;x<5;x++)
  {
    ListePions[k].actif = 1;
    ListePions[k].couleur = BLANC;
    ListePions[k].x = 2*x + y%2;
    ListePions[k].y = y;
    k = k+1;
  }
}

for(y=6;y<10;y++)
{
  for(x=0;x<5;x++)
  {
    ListePions[k].actif = 1;
    ListePions[k].couleur = NOIR;
    ListePions[k].x = 2*x + y%2;
    ListePions[k].y = y;
    k = k+1;

  }
}


}



int cherchePion(int xg,int yg)
{
  int k;
  for(k=0;k<50;k=k+1)
  {
    if(ListePions[k].actif == 1 && ListePions[k].x == xg && ListePions[k].y == yg)
    {
      return k;
    }
  }

  return -1;
}

void placePion(pion* p)
{
  int k;
  for(k=0;k<50;k=k+1)
  {
    if(ListePions[k].actif == 0)
    {
      ListePions[k] = *p;
      ListePions[k].actif = 1;
      return;
    }
  }
}
