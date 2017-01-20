/*Note: cpp files aren't in classes, probably should do that. */
#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include "headers/Mesh.h"
#include "headers/objloader.h"
#include "headers/Shader.h"

#include "headers/Camera.h"
//#include "trimesh.h"

// --------------------------------------------------------------------------
// Variables
vector<vec3> _colours;
vector<vec3> _vertices;
vector<vec2> _uv;
vector<GLushort> _faces;
vector<vec3> _normals;

int width = 512, height = 512;

float _rotation_y;
mat4 _modelview;
mat4 _projection;
mat4 winRatio = mat4(1.f);
Camera camera;
vec3 _lightSource = vec3(0.f, 1.f, 0.f);
//TriMesh* _mesh;
//std::vector<unsigned int> _triangleIndices;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

void RotateObject(float factor);
void PrintDirections();
// --------------------------------------------------------------------------
#endif