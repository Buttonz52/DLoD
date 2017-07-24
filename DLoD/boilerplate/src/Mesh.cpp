#include "GameEngine/Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	DestroyMesh();
}

//Reads object file using Assimp and converts to vectors of respective types.
//Resources: https://learnopengl.com/#!Model-Loading/Model
//https://nickthecoder.wordpress.com/2013/01/20/mesh-loading-with-assimp/

bool Mesh::Initialize() {

	/* Initialization of buffers for mesh goes here */
	colours.clear();
	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;
	const GLuint NORMAL_INDEX = 2;
	const GLuint UV_INDEX = 3;

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_INDEX);

	for (int i = 0; i < vertices.size(); i++)
		colours.push_back(colour);
	// create another one for storing our colours
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(vec3), colours.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(COLOUR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(COLOUR_INDEX);

	// create another one for storing our colours
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(NORMAL_INDEX);

	// create another one for storing our uv
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec3), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(UV_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(UV_INDEX);

	//Indices buffer.
	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLushort), faces.data(), GL_STATIC_DRAW);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}
void Mesh::UpdateColour(const vec3 &colour) {
	colours.clear();
	for (int i = 0; i < vertices.size(); i++) {
		colours.push_back(colour);
	}
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colours.size() * sizeof(vec3), colours.data());
}

bool Mesh::ReadMesh(const string &filename) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | 
															aiProcess_FlipUVs | 
													aiProcess_FixInfacingNormals|
													aiProcess_GenSmoothNormals|
													aiProcess_JoinIdenticalVertices
													);
 
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Error Assimp: " << importer.GetErrorString() << endl;
		return false;
	}

	aiMesh *mesh = scene->mMeshes[0];
	int meshFaces = mesh->mNumFaces;

	const aiVector3D empty(0.f, 0.f, 0.f);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		const aiVector3D pVertex = mesh->mVertices[i];
		const aiVector3D pNormal = mesh->mNormals[i];
		const aiVector3D pUV = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : AddUV(pVertex, filename);
		
		vec3 _vertex(pVertex.x, pVertex.y, pVertex.z);
		vertices.push_back(_vertex);

		vec3 _normal(pNormal.x, pNormal.y, pNormal.z);
		normals.push_back(_normal);	

		vec3 _uv(pUV.x, pUV.y, pUV.z);
		uvs.push_back(_uv);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);

		faces.push_back(face.mIndices[0]);
		faces.push_back(face.mIndices[1]);
		faces.push_back(face.mIndices[2]);
	}
	elementCount = faces.size();

	return true;
}

aiVector3D Mesh::AddUV(const aiVector3D &vertex, const string &type) {

	float theta;
	float phi;
	float r = sqrt(vertex.x*vertex.x + vertex.y*vertex.y + vertex.z*vertex.z);
	if (r == 0.f) {
		theta = 0.f;
		phi = 0.f;
	}
	else {
		//No seam, but texture is doubled and looks kind of funny at edges.
		if (type == "models/plane.obj")
		{
			theta = asin(vertex.x / r) / (PI)+0.5f;
			phi = (acos(vertex.z / r)) / (PI);
		}
		else //spherical coords
		{
			theta = asin(vertex.x / r) / (PI)+0.5f;
			phi = (acos(vertex.y / r)) / (PI);
		}
	}
	return aiVector3D(theta, phi, r);
}

void Mesh::AddColour(const vec3 &colour) {
	this->colour = colour;
}

//Clears all vectors
void Mesh::ClearMesh() {
	colours.clear();
	vertices.clear();
	uvs.clear();
	faces.clear();
	normals.clear();
}

void Mesh::DestroyMesh() {
	ClearMesh();
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indicesBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &colourBuffer);

	glDeleteVertexArrays(1, &vertexArray);
}