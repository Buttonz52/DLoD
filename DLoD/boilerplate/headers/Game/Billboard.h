#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Utility.h"
#include "GameEngine\Texture.h"
#include "GameEngine\Shader.h"

class Billboard
{
public:
	mat4 modelMatrix, viewMatrix;
	Billboard();
	~Billboard();

	void setPosition(const vec3 & p);

	vec3 & getPosition();

	void setScale(const vec3 & s);

	vec3 & getScale();

	void Destroy();

	void updateModelMatrix();

	void calculateViewMatrix(const vec3 & camUp, const vec3 & campos);

	vec3 Normalize(const vec3 & v);

	void InitializeShaders(const string & vert, const string & frag);

	bool initTexture(const string & filename, GLuint target);

	bool GenerateSquareVertices(float scale_x, float scale_y, const vec3 & col);

	bool GenerateVertices(const vector<vec3>* verts, const vec3 & col, const vector<vec2>* uv);

	bool Initialize();

	void Render(GLuint type, const mat4 & projMatrix, const vec3 & camUp, const vec3 & campos);

private:
	int mixColour, hasTexture;

	float rotateZ, transparency, mixAmount;

	vec3 _scale, position, colour;



	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint colourBuffer;
	GLsizei elementCount;

	vector<vec3> vertices, colours;
	vector<vec2> uvs;

	Texture mainTexture;
	Shader mainShader;

};
#endif