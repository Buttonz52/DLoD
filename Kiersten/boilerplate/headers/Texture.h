#ifndef TEXTURE_H
#define TEXTURE_H

#include "Utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Texture
{
public:
	Texture();
	~Texture();

	GLuint textureID;
	GLuint target;
	int width;
	int height;
	bool InitializeTexture(const char* filename, GLuint target);
	void DestroyTexture();
private:

};
#endif

