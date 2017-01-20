#include "headers/Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indicesBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &colourBuffer);

	glDeleteVertexArrays(1, &vertexArray);
}

bool Mesh::ReadMesh(const string &filename) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	 
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
		const aiVector3D pUV = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : empty;
		
		vec3 _vertex(pVertex.x, pVertex.y, pVertex.z);
		vertices.push_back(_vertex);

		vec3 _normal(pNormal.x, pNormal.y, pNormal.z);
		normals.push_back(_normal);

		vec2 _uv(pUV.x, pUV.y);
		uvs.push_back(_uv);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);

		faces.push_back(face.mIndices[0]);
		faces.push_back(face.mIndices[1]);
		faces.push_back(face.mIndices[2]);
	}
	//delete mesh;
	//delete scene;
	return true;
}

void Mesh::AddTexture(const char *filename) {
	//if (!texture.InitializeTexture(filename, GL_TEXTURE_2D)) {
	//	cout << "Error with mesh: Failed to initialize texture!" << endl;
	//}
}
void Mesh::Initialize() {

	/* Initialization of buffers for mesh goes here */
}

void Mesh::Render() {
	/* Rendering code for mesh goes here */
}

