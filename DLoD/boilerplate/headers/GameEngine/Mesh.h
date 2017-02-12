#ifndef MESH_H
#define MESH_H

#include "../Game/Utility.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "Texture.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Initialize();
	bool ReadMesh(const string &filename);

	void AddColour(const vec3 &colour);
	void ClearMesh();
	void DestroyMesh();

	// Variables
	vector<vec3> colours;
	vector<vec3> vertices;
	vector<vec3> uvs;
	vector<GLushort> faces;
	vector<vec3> normals;
	GLuint vertexArray;
	GLsizei elementCount;
	
private:

	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indicesBuffer;
	GLuint textureBuffer;
	GLuint colourBuffer; //might not need

	aiVector3D AddUV(const aiVector3D &vertex, string filename);
};
#endif
