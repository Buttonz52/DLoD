#ifndef MESH_H
#define MESH_H
#include "Utility.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "Texture.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	/*struct CameraInfo {
		mat4 _view;
		mat4 _projection;
		mat4 winRatio;
		vec3 _lightSource;

		CameraInfo() : _view(mat4(1.f)), _projection(mat4(1.f)), winRatio(mat4(1.f)), _lightSource(vec3(0))
		{}
	};*/
	//CameraInfo cameraInfo;
	bool ReadMesh(const string &filename);
	bool Initialize();
	//Render not working with camera right now.
	//void Mesh::Render(MyShader *shader, CameraInfo *c);
	void AddTexture(const char *filename);
	void AddColour(vec3 *colour);
	void ClearMesh();
	void DestroyMesh();
	// Variables
	vector<vec3> colours;
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<GLushort> faces;
	vector<vec3> normals;
	GLuint vertexArray;
	GLsizei elementCount;
	//Texture texture;
private:

	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indicesBuffer;
	GLuint textureBuffer;
	GLuint colourBuffer; //might not need

	//GLuint vertexArray;


	mat4 _view;
	mat4 _projection;
};
#endif
