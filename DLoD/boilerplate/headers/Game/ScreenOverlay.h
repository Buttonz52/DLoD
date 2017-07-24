#pragma once
#include "Utility.h"
#include "GameEngine/Texture.h"
#include "GameEngine/Shader.h"
class ScreenOverlay
{
public:
	ScreenOverlay();
	~ScreenOverlay();

	int isFontTex;
	void InitializeShaders(const string &vert, const string &frag);
	bool InitTexture(const string &filename, GLuint target);
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

	void InitVideo();
	void SetTexture(Texture *tex);
	void UpdateGameText(const string & text, const int & kerning);
	void UpdateUVBuffer(const vector<vec2>* uvs);
	void UpdateVertexBuffer(const vector<vec3>* vertices);

	void Render(GLuint type, const vec3 &colour);

	void SetColour(const vec3 &c);
	void UpdateColourBuffer(const vec3 & colour);
	void UpdateColourBuffer2(const vector<vec3>& colours);
	vec3 & getColour();

	void SetPosition(const vec3 &pos);
	vec3 & GetPosition();

	void SetRotateZ(const float &r);

	void SetHasTexture(const bool &);
	int GetHasTexture();

	void SetMixFlag(const bool &);
	int GetMixFlag();

	void SetMixAmount(const float &);
	float GetMixAmount();

	void SetTransparency(const float &);
	float GetTransparency();

	void SetScale(const vec3 &s);
	vec3 & GetScale();

	void Destroy();

	vec3 Normalize(const vec3 &v);

private:
	int mixColour, hasTexture;

	float rotateZ, transparency, mixAmount;

	vec3 _scale, position, colour;
	bool updateColours;
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint colourBuffer;
	GLsizei elementCount;

	vector<vec3> vertices, colours;
	vector<vec2> uvs;

	Texture texture;
	Shader shader;
};

