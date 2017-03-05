#pragma once
#include "Utility.h"
#include "../GameEngine/Texture.h"
#include "../GameEngine/Shader.h"
#include "../GameEngine/GlyphExtractor.h"

class ScreenOverlay
{
public:
	float updateFactor;
	void InitializeShaders(const string &vert, const string &frag);
	bool initTexture(const string &filename, GLuint target);
	bool GenerateSquareVertices(const float scale_x, const float scale_y, const vec3 &col);
	bool GenerateVertices(const vector<vec3> *verts, const vec3 &colour, const vector<vec2> *uv);
	void UpdateBuffers(vector<vec2> *uvs);
	bool Initialize();
	void Render(GLuint type);
	void Destroy();
	void setColour(const vec3 &c);
	vec3 & getColour();
	void setPosition(const vec3 &pos);
	vec3 & getPosition();
	void setRotateZ(const float &r);
	void setHasTexture(const bool &);
	int getHasTexture();
	void setMixColour(const bool &);
	int getMixColour();
	void setScale(const vec3 &s);
	vec3 & getScale();
	bool InitQuad(const string &tex,
		const string &vert,
		const string &frag,
		const float x,
		const float y,
		const vec3 &col);

	ScreenOverlay();
	~ScreenOverlay();
	
	//Holds glyph data
	
private:
	vec3 Normalize(const vec3 &v);
	GLuint vertexArray;
	GLsizei elementCount;

	GLuint vertexBuffer;
	GLuint textureBuffer;
	float rotateZ;
	vec3 _scale;
	vector<vec3> vertices;
	vector<vec2> uvs;
	vec3 position;
	vec3 colour;
	Texture texture;
	Shader shader;
	int hasTexture;
	int mixColour;
};

