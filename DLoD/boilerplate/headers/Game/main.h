/*Note: cpp files aren't in classes, probably should do that. */
#ifndef MAIN_H
#define MAIN_H

#include "../GameEngine/Mesh.h"
#include "../GameEngine/Shader.h"
#include "Utility.h"
#include "../GameEngine/Texture.h"
//#include "../GEO/Camera.h"
#include "../GEO/Plane.h"
#include "../GEO/player/Player.h"
#include "Audio.h"
#include <dirent.h>

using namespace std;
// --------------------------------------------------------------------------
// Variables
vector<vec3> _colours;
vector<vec3> _vertices;
vector<vec2> _uv;
vector<GLushort> _faces;
vector<vec3> _normals;
vector<Mesh> meshes;

int width = 512, height = 512;

double mouse_old_x, mouse_old_y;
float _translate_z = 1.0;
float _rotate_x = 0.0;
float _rotate_y = 0.0;

mat4 _view;
mat4 _projection;
mat4 winRatio = mat4(1.f);
Camera camera;
Player curPlayer;
Audio audio;
vec3 _lightSource = vec3(0.f, 1.f, 2.f);
//Audio music;

// OpenGL utility and support function prototypes
void PrintDirections();

// Mesh functions
int LoadAllObjFiles(const char *pathname);	
void AddMesh(const string *pathname, const string * filename);
// --------------------------------------------------------------------------
#endif