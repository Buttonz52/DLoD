/*
	Header file for texture loader code from boilerplate.

*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Game/Utility.h"

class Texture {
public:
	Texture();
	~Texture();

	GLuint textureID;
	GLuint target;
	int width;
	int height;

	bool InitializeTexture(const string &filename, GLuint target);
	bool InitializeSkybox(const vector<string> &filename);
	bool BindTexture(GLuint program, const string &varName);
	void UnbindTexture();
	void DestroyTexture();
};
#endif