#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "grille.h"

typedef struct {
		unsigned char* pixels;
		int width;
		int height;
} Image;

Image* loadBMP(const char* filename);

#endif
