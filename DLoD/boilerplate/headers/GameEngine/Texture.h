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
	bool InitializeTexture(const string &filename, GLuint target);
	bool Texture::InitializeSkybox(const vector<string> &filename);
	void DestroyTexture();
	bool BindTexture(GLuint program, string varName);
	void UnbindTexture(GLuint format);
	// --------------------------------------------------------------------------
	// Functions to set up OpenGL buffers for storing textures
};
#endif