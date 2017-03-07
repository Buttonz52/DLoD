#include "GameEngine/Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

//Reads object file using Assimp and converts to vectors of respective types.
//Resources: https://learnopengl.com/#!Model-Loading/Model
//https://nickthecoder.wordpress.com/2013/01/20/mesh-loading-with-assimp/

bool Mesh::Initialize(bool hasTangents) {

	/* Initialization of buffers for mesh goes here */

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

	if (hasTangents) {
		glGenBuffers(1, &tangentBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
		glBufferData(GL_ARRAY_BUFFER, tangentMesh.size() * sizeof(vec3), tangentMesh.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(4);
	}
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
		//This creates a seam
		//	theta = 0.5f*((atan2(vertex.x,vertex.z)/PI)+1.0f);
		//	phi = 0.5f + asin(-vertex.y)/PI;

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

/** Calculates tangent coordinates for mesh. */
void Mesh::calculateMeshTangent() {
	tangentMesh.clear();	//Clear previous tangent coordinates.

	for (int i = 0; i < vertices.size(); i += 3) {
		//Get vertices of triangle.
		glm::vec3 v0(vertices[i]);
		glm::vec3 v1(vertices[i + 1]);
		glm::vec3 v2(vertices[i + 2]);


		glm::vec3 e1 = v1 - v0;
		glm::vec3 e2 = v2 - v0;

		glm::vec3 Duv1 = uvs[i + 1] - uvs[i];
		glm::vec3 Duv2 = uvs[i + 2] - uvs[i];

		GLfloat fractal;
		float denom = (Duv1.x * Duv2.y - Duv1.y*Duv2.x);
		if (denom != 0.f) {	//Check that not dividing by 0.
			fractal = 1.f / denom;
		}

		else {
			fractal = 0.f;
		}
		vec3 tan1;
		tan1.x = fractal * (e1.x * Duv2.y + e2.x * (-Duv1.y));
		tan1.y = fractal * (e1.y * Duv2.y + e2.y * (-Duv1.y));
		tan1.z = fractal * (e1.z * Duv2.y + e2.z * (-Duv1.y));

		//Push back tangent for each vertex of triangle.
		tangentMesh.push_back(tan1);
		tangentMesh.push_back(tan1);
		tangentMesh.push_back(tan1);
	}
}

void Mesh::AddColour(const vec3 &colour) {
	colours.clear();
	if (faces.size() == 0) {
		cout << "ERROR MESH: Error loading colours, faces.size() = 0." << endl;
	}
	else {
		for (int i = 0; i < faces.size(); i++) {
			colours.push_back(colour);
		}
	}
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
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indicesBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &tangentBuffer);
	glDeleteBuffers(1, &colourBuffer);

	glDeleteVertexArrays(1, &vertexArray);
}