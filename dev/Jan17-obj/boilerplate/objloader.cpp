#include "headers/objloader.h"
#include <boost/algorithm/string/replace.hpp>
#include <string>
#include "TriMesh.h"
#include <assimp/Importer.hpp>

//#pragma comment(lib, \'assimp.lib\')

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

/* objloader that doesn't quite work... */
ObjLoader::ObjLoader() {}
ObjLoader::~ObjLoader() {}
/* Loads the vertices, normals, faces, and uv coordinates of obj file.  Might not work correctly. */
void ObjLoader::LoadObject(const char *filename, vector<vec3> &vertices, vector<vec3> &normals, vector<GLushort> &faces, vector<vec2> &uv) {
	glm::vec3 v;
	//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl; exit(1);
	}

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			istringstream s(line.substr(2));
			s >> v.x; s >> v.y; s >> v.z;
			vertices.push_back(v);
		}

		else if (line.substr(0, 2) == "f ")
		{
			istringstream s(line.substr(2));

			//The error for sphere occurs here.  In it's file format, it has faces as:
			//	face//normal 
			//	such that the normal is correlated with the face.  This is not the case with
			//the teapot.

			cout << line << endl;
			GLushort a, b, c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			faces.push_back(a);
			faces.push_back(b);
			faces.push_back(c);

		}
		else if (line.substr(0, 3) == "vt ")
		{
			istringstream s(line.substr(3));
			glm::vec2 w; s >> w.x; s >> w.y;
			uv.push_back(w);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			istringstream s(line.substr(3));
			s >> v.x; s >> v.y; s >> v.z;
			normals.push_back(v);
		}
		else if (line[0] == '#')
		{
			// ignoring this line 
		}
		else
		{
			/* ignoring this line */
		}
	}
}


	/*normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = 0; i < elements.size(); i += 3)
	{
		GLushort ia = elements[i];
		GLushort ib = elements[i + 1];
		GLushort ic = elements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
			glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
		normals[ia] = normals[ib] = normals[ic] = normal;
	}*/
