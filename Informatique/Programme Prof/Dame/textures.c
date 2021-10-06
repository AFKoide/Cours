#include <stdio.h>
#include <GLUT/glut.h>
#include <stdlib.h>

#include "textures.h"



GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image.largeur, image.hauteur,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image.pixels);
	return textureId;
}
