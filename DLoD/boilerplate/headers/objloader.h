/*
	Not fully working, but this is where obj file loading stuff can go.

*/
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <fstream>
#include <sstream>
#include "headers/Utility.h"

class ObjLoader {
public:
	ObjLoader();
	~ObjLoader();
	void ObjLoader::LoadObject(const char *filename, vector<vec3> &vertices, vector<vec3> &normals, vector<GLushort> &faces, vector<vec2> &uv);	//This might not fully work?
private:
};
#endif