#pragma once
#include "Utility.h"
#include "GameEngine/Texture.h"
#include "GameEngine/Shader.h"

class ScreenOverlay
{
public:
	ScreenOverlay();
	~ScreenOverlay();

	int isFontTex, isRedTransparent;

	void InitializeShaders(const string &vert, const string &frag);
	bool initTexture(const string &filename, GLuint target);
	bool InitQuad(const string &tex,
		const string &vert,
		const string &frag,
		const float x,
		const float y,
		const vec3 &col);
	void InitializeGameText(const string & text, const vec3 & position, const vec3 & colour, int kerning);

	bool Initialize();

	bool GenerateSquareVertices(const float scale_x, const float scale_y, const vec3 &col);
	bool GenerateVertices(const vector<vec3> *verts, const vec3 &colour, const vector<vec2> *uv);
	bool GenerateBorder(float scale_x, float scale_y, const float & thicc, const vec3 & col, const vec3 & position);
	void GenerateTextUVs(vector<vec2>& uvs, const char & ch);

	void UpdateBuffers(const vector<vec2> *uvs);
	void UpdateVertices(const vector<vec3> *vertices);
	void UpdateGameText(const string & text);

	void Render(GLuint type, const vec3 &colour);

	void setColour(const vec3 &c);
	vec3 & getColour();

	void setPosition(const vec3 &pos);
	vec3 & getPosition();

	void setRotateZ(const float &r);

	void setHasTexture(const bool &);
	int getHasTexture();

	void setMixFlag(const bool &);
	int getMixFlag();

	void setMixAmount(const float &);
	float getMixAmount();

	void setTransparency(const float &);
	float getTransparency();

	void setScale(const vec3 &s);
	vec3 & getScale();

	void Destroy();

	vec3 Normalize(const vec3 &v);



	//Holds glyph data

private:
	int mixColour, hasTexture;

	float rotateZ, transparency, mixAmount;

	vec3 _scale, position, colour;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLsizei elementCount;

	vector<vec3> vertices;
	vector<vec2> uvs;

	Texture texture;
	Shader shader;
};

