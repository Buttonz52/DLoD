#pragma once
#include "GEO.h"
#include "Game/ScreenOverlay.h"
class Shadow: public GEO
{
public:
	GLuint SHADOW_WIDTH, SHADOW_HEIGHT;
	GLuint depthMapFBO;
	//GLuint depthMap;
	Shader *shadowShader;
	Shadow();
	~Shadow();
	void initShadow();
	void Render(Shader &shader,const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource);
	void Destroy();
	void endRender();
	void bindForWriting();
	void display();
	void bindForReading(Shader * shader);
	void unbindTexture();
	//void bindForReading(GLenum TexUnit);
};

