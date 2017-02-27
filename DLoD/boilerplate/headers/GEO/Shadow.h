#pragma once
#include "GEO.h"
class Shadow: public GEO
{
public:
	GLuint SHADOW_WIDTH, SHADOW_HEIGHT;
	GLuint depthMapFBO;
	GLuint depthMap;

	Shadow();
	~Shadow();
	void initShadow();
	void Render(Shader &shader,const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource);
	void Destroy();
	void endRender();
	void bindForWriting();
	void bindForReading(GLenum TexUnit);
};

