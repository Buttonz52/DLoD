#pragma once
#include "../GameEngine/GlyphExtractor.h"
#include "../Game/Utility.h"
#include "../GameEngine/Shader.h"
#include "../GameEngine/Mesh.h"

class Text
{
public:
	Shader shader;
	Text();
	~Text();
	void InitText(const string &text, const vec3 & colour);
	void AddShaders(const string &vert, const string &frag);
	Mesh mesh;
	void Render(GLuint type, const vec3 &position, const float _scale);
	void segLerp(MySegment *segment, float adv);
	vec3 lerp(const vec3 &a, const vec3 &b, float t);
	vec3 Normalize(const vec3 &);
};

