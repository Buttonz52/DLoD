/* 
	Shader header file with methods from boilerplate.
*/
#ifndef MYSHADER_H
#define MYSHADER_H

#include "headers/Utility.h"

class Shader
{
public:
	// OpenGL names for vertex and fragment shaders, shader program
	GLuint  vertex;
	GLuint  fragment;
	GLuint  program;

	bool InitializeShaders(const string &vert, const string &frag);
	void DestroyShaders();
	GLuint CompileShader(GLenum shaderType, const string &source);
	string LoadSource(const string &filename);
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};
#endif