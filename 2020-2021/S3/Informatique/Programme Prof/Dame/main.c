
#include <stdio.h>
#include <GL/glut.h>
#include "grille.h"
#include "affichage.h"
#include "souris.h"
#include "clavier.h"




int main(int argc, char **argv)
{
//  printf("Debut\n");
glutInit(&argc, argv);
glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
//printf("Initialisation faite\n");

glEnable(GL_TEXTURE_2D);

glutInitWindowPosition(100,100);
glutInitWindowSize(600,600);
glutCreateWindow ("square");
//printf("Fenetre ouverte \n");


glClearColor(0.0, 0.0, 0.0, 0.0); // black background
glMatrixMode(GL_PROJECTION); // setup viewing projection
glLoadIdentity(); // start with identity matrix
glOrtho(0.0, 10.0, 0.0, 10.0, -1.0, 1.0); // setup a 10x10x2 viewing world
//printf("OpenGL paramétré \n");



preparePions();
Dragged.actif = 0;
//printf("Jeu initialisé \n");


glutDisplayFunc(display);
glutMouseFunc(mouse);
glutMotionFunc(mouseDrag);
glutKeyboardFunc (keyboard);
//printf("Callbacks renseignés \n");


glutMainLoop();
//printf("Fin de la boucle d'événements \n");


return 0;
}
