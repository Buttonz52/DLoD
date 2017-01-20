#ifndef MESH_H
#define MESH_H
#include "Utility.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include "Texture.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	bool ReadMesh(const string &filename);
	void Initialize();
	void Render();
	void AddTexture(const char *filename);
	// Variables
	vector<vec3> colours;
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<GLushort> faces;
	vector<vec3> normals;
	//Texture texture;
private:

	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indicesBuffer;
	GLuint textureBuffer;
	GLuint colourBuffer; //might not need

	GLuint vertexArray;
};
#endif
