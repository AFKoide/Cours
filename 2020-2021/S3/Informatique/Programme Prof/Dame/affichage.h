#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

#include "grille.h"

extern pion Dragged;
extern float  xDraggedGL,yDraggedGL;

void carre(float x0,float y0,float a );
void cercle(float xc,float yc,float R);
void affichePionGL(float xGL,float yGL,int couleur);
void affichePion(int xg, int yg,int couleur);
void damier();
void affichePions();
void display(void);
void mouse2grid(int xs,int ys,int* xg,int*yg);
void mouse2GL(int xs,int ys,float* xGL,float*yGL);

#endif
