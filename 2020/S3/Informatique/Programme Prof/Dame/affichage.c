#include <stdio.h>
#include <GLUT/glut.h>
#include <math.h>
#include "affichage.h"

pion Dragged;
float  xDraggedGL,yDraggedGL;

void carre(float x0,float y0,float a )
{
  glBegin(GL_POLYGON);
  glVertex3f(x0, y0, 0.0);
  glVertex3f(x0+a, y0, 0.0);
  glVertex3f(x0+a, y0+a, 0.0);
  glVertex3f(x0, y0+a, 0.0);
  glEnd();

}
void cercle(float xc,float yc,float R)
{
int nbpoints = 50;
int k;
float dalpha = 2*M_PI/nbpoints;
float x,y;

glBegin(GL_POLYGON);
for(k=0;k<nbpoints;k=k+1)
{
x = R*cos(k*dalpha) + xc;
y = R*sin(k*dalpha) + yc;
glVertex3f(x, y, 0.0);
}
glEnd();

}

void affichePionGL(float xGL,float yGL,int couleur)
{
  if(couleur == NOIR)
  {glColor3f(0,0, 1);
  }
  else
  {glColor3f(1,0, 0);
  }
  cercle(xGL,yGL,0.4);

}


void affichePion(int xg, int yg,int couleur)
{
affichePionGL(xg+0.5,yg+0.5,couleur);
}

void damier()
{
int x,y;
for(y=0;y<10;y=y+1)
  {
    for(x=0;x<10;x=x+1)
    {if((x+y)%2==1)
      {
        glColor3f(0.35, 0, 0.0);
      }
      else
      {
        glColor3f(0,0, 0.35);
      }
      carre(x,y,1);
    }
  }
}

void affichePions()
{ int k;
  for(k=0;k<50;k++)
  {
    if(ListePions[k].actif ==1)
    {

      affichePion(ListePions[k].x,ListePions[k].y,ListePions[k].couleur);

    }
  }

}


void display(void)
{
//printf("appel a Display\n");

glClear( GL_COLOR_BUFFER_BIT);

damier();
affichePions();
if(Dragged.actif == 1)
{affichePionGL(xDraggedGL,yDraggedGL,Dragged.couleur);
}
glFlush();
}


//-----------------------------------------------
// mouse2grid : convertit les coordonnées souris (en pixels)
// vers des coordonnées "damier"
// entrees : xs,ys = coordonnées souris en pixels
// sorties : xg,yg = coordonnées damier en "cases"
// retour : rien
//-----------------------------------------------
void mouse2grid(int xs,int ys,int* xg,int*yg)
{
  *xg = (xs*10.0)/glutGet(GLUT_WINDOW_WIDTH);
  *yg = 10.0 - (ys*10.0)/glutGet(GLUT_WINDOW_HEIGHT);
}

void mouse2GL(int xs,int ys,float* xGL,float*yGL)
{
  *xGL = (xs*10.0)/glutGet(GLUT_WINDOW_WIDTH);
  *yGL = 10.0 - (ys*10.0)/glutGet(GLUT_WINDOW_HEIGHT);
}
