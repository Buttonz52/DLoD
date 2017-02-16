#pragma once
#include "Utility.h"
#include "../GameEngine/Texture.h"
#include "../GameEngine/Shader.h"

class ScreenOverlay
{
public:

	GLuint vertexArray;
	GLsizei elementCount;

	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint colourBuffer; //might not need
	float rotateZ;
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> colours;
	vec3 position;
	vec3 colour;
	Texture texture;
	Shader shader;
	int hasTexture;
	void InitializeShaders(const string &vert, const string &frag);
	bool GenerateVertices(vector<vec3> *verts, vector<vec3> *colour, vector<vec2> *uv);
	bool initTexture(const string &filename, GLuint target);
	bool GenerateSquareVertices(const float scale_x, const float scale_y);
	bool GenerateVertices(const vector<vec3> *verts, const vector<vec3> *colour, const vector<vec2> *uv);
	bool Initialize();
	void Render(GLuint type);
	void Destroy();
	void setPosition(const vec3 &pos);
	void setRotate(const float &r);
	vec3 Normalize(const vec3 &v);
	ScreenOverlay();
	~ScreenOverlay();
};

