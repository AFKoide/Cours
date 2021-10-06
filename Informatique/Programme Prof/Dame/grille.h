#ifndef __GRILLE_H__
#define __GRILLE_H__

#define NOIR 1
#define BLANC 2

typedef struct {
  int x;
  int y;
  int couleur;
  int actif;
} pion;

extern pion ListePions[50];



void preparePions();
int cherchePion(int xg,int yg);
void placePion(pion* p);


#endif
