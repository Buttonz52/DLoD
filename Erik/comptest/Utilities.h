
#ifndef utilities_h
#define utilities_h

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <OpenGL/gl3.h>
#include <vector>
#include <glm.hpp>

using namespace std;


GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
string LoadSource(const string &filename);
bool CheckGLErrors();
void QueryGLVersion();
void pushVec3(vector<GLfloat>* points, glm::vec3 vec);
void pushVec2(vector<GLfloat>* points, glm::vec2 vec);


#endif /* utilities_h */
