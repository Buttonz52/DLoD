/*
	Header file for texture loader code from boilerplate.

*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Game/Utility.h"

class Texture {
public:
	GLuint textureID;
	GLuint target;
	int width;
	int height;
	Texture();
	~Texture();
	bool InitializeTexture(const char* filename, GLuint target);
	bool InitializeSkybox(vector<const char*>* filename, GLuint target);
	void DestroyTexture();
	bool BindTexture(GLuint program, GLuint target, string varName);
	void UnbindTexture(GLuint format);
	// --------------------------------------------------------------------------
	// Functions to set up OpenGL buffers for storing textures
};
#endif