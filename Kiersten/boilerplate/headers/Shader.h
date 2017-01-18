/* 
	Shader header file with methods from boilerplate.
*/
#ifndef MYSHADER_H
#define MYSHADER_H

#include "headers/Utility.h"

struct MyShader
{
	// OpenGL names for vertex and fragment shaders, shader program
	GLuint  vertex;
	GLuint  fragment;
	GLuint  program;

	// initialize shader and program names to zero (OpenGL reserved value)
	MyShader() : vertex(0), fragment(0), program(0)
	{}
};

bool InitializeShaders(MyShader *shader);
void DestroyShaders(MyShader *shader);
GLuint CompileShader(GLenum shaderType, const string &source);
string LoadSource(const string &filename);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
#endif
