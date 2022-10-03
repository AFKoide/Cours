#include <stdio.h>
#include <GL/glut.h>
#include <math.h>


#define NOIR 1
#define BLANC 2

typedef struct {
  int x;
  int y;
  int couleur;
  int actif;
} pion;

pion ListePions[50];



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

void affichePion(int xg, int yg)
{
  cercle(xg+0.5,yg+0.5,0.4);
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

void affichePions()
{ int k;
  for(k=0;k<50;k++)
  {
    if(ListePions[k].actif ==1)
    {
      if(ListePions[k].couleur == NOIR)
      {glColor3f(0,0, 1);
      }
      else
      {glColor3f(1,0, 0);
      }
      affichePion(ListePions[k].x,ListePions[k].y);

    }
  }

}


void display(void)
{
printf("appel a Display\n");

glClear( GL_COLOR_BUFFER_BIT);

damier();
affichePions();

glFlush();
}

void mouse2grid(int xs,int ys,int* xg,int*yg)
{
  *xg = (xs*10.0)/glutGet(GLUT_WINDOW_WIDTH);
  *yg = 10.0 - (ys*10.0)/glutGet(GLUT_WINDOW_HEIGHT);
}


void mouse(int button, int state,int xs, int ys)
{int xg,yg;
  mouse2grid(xs,ys,&xg,&yg);

  glutPostRedisplay();
}



int main(int argc, char **argv)
{
printf("hello world\n\n");
glutInit(&argc, argv);
glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

glutInitWindowPosition(100,100);
glutInitWindowSize(600,600);
glutCreateWindow ("square");

glClearColor(0.0, 0.0, 0.0, 0.0); // black background
glMatrixMode(GL_PROJECTION); // setup viewing projection
glLoadIdentity(); // start with identity matrix
glOrtho(0.0, 10.0, 0.0, 10.0, -1.0, 1.0); // setup a 10x10x2 viewing world

preparePions();


glutDisplayFunc(display);
glutMouseFunc(mouse);

glutMainLoop();

return 0;
}
