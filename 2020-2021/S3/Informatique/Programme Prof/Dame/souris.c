#include <stdio.h>
#include <GLUT/glut.h>
#include "grille.h"
#include "affichage.h"


void mouseDrag(int xs,int ys)
{

mouse2GL(xs,ys,&xDraggedGL,&yDraggedGL);
//printf("mouseDrag : %f %f %d\n",xDraggedGL,yDraggedGL,Dragged.couleur);
glutPostRedisplay();

}


void mouse(int button, int state,int xs, int ys)
{int xg,yg;
  mouse2grid(xs,ys,&xg,&yg);
  //printf("mouse : %d %d %d %d\n",button,state,xg,yg);
  if(button == GLUT_LEFT_BUTTON )
  { if( state == GLUT_DOWN)
    {int trouve = cherchePion(xg,yg);
      if(trouve != -1)
      {
        Dragged = ListePions[trouve];
        Dragged.actif = 1;

      //printf("click %d %d %d\n",xg,yg,ListePions[trouve].couleur);
        ListePions[trouve].actif = 0;
        mouseDrag(xs,ys);
      }
    }
    else {
      int trouve = cherchePion(xg,yg);
if( Dragged.actif ==1)
          {if(trouve == -1 )
            {
             Dragged.x = xg;
             Dragged.y = yg;
             }
            placePion(&Dragged);
             Dragged.actif = 0;
            glutPostRedisplay();

          }


    }
  }
}
