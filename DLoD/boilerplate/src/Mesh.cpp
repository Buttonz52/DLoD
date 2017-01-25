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
		const aiVector3D pUV = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : AddUV(pVertex);
		
		vec3 _vertex(pVertex.x, pVertex.y, pVertex.z);
		vertices.push_back(_vertex);

		vec3 _normal(pNormal.x, pNormal.y, pNormal.z);
		normals.push_back(_normal);	//normals are negative???? but multiplying my -1 scalar fixes it -bp

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
	//delete mesh;
	//delete scene;
	//cout << "Loaded " << filename << endl;
	return true;
}

void Mesh::AddColour(vec3 *colour) {
	colours.clear();
	if (faces.size() == 0) {
		cout << "ERROR MESH: Mesh not loaded to add colour." << endl;
	}
	else {
		for (int i = 0; i < faces.size(); i++) {
			colours.push_back(*colour);
		}
	}
}
void Mesh::AddTexture(const char *filename) {
	//if (!texture.InitializeTexture(filename, GL_TEXTURE_2D)) {
	//	cout << "Error with mesh: Failed to initialize texture!" << endl;
	//}
}
bool Mesh::Initialize() {

	/* Initialization of buffers for mesh goes here */
	elementCount = faces.size();
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

//Render isn't working with camera at the moment: Issues with updating
//matrices or something.
/*void Mesh::Render(MyShader *shader, CameraInfo *c) {
	// Rendering code for mesh goes here 
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader->program);
	glBindVertexArray(vertexArray);

	vec3 fp = vec3(0, 0, 0);



	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "modelview"), 1, GL_FALSE, glm::value_ptr(c->_view));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(c->_projection));
	glUniform3fv(glGetUniformLocation(shader->program, "lightPosition"), 1, glm::value_ptr(c->_lightSource));

	//Drawing obj files doesn't work yet.
	//glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

	//JAN 16: This works now. //This is in Allan's code, but nothing appears.  TODO: Fix it.
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}*/

aiVector3D Mesh::AddUV(aiVector3D vertex) {
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
		theta = asin(vertex.x / r) / (PI)+0.5f;
		phi = (acos(vertex.y / r)) / (PI);
	}
	return aiVector3D(theta, phi, r);
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
	glDeleteBuffers(1, &colourBuffer);

	glDeleteVertexArrays(1, &vertexArray);
	glDeleteProgram(program);
	//Destroy the shader
	shader.DestroyShaders();
}