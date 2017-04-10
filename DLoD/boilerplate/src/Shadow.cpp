#include "GEO\Shadow.h"

//code mainly from: https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping

Shadow::Shadow()
{
	SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
}


Shadow::~Shadow()
{
}

void Shadow::Destroy() {
	glDeleteFramebuffers(1,&depthMapFBO);
	glDeleteTextures(1,&depthMap);
}
void Shadow::Render(Shader &shader,const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource) {
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//RenderScene(simpleDepthShader); //render all objects
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::endRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Shadow::initShadow() {

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		cout << "Error with framebuffer" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::bindForWriting() {
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::bindForReading(Shader *shader) {
	glActiveTexture(GL_TEXTURE0 + depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(glGetUniformLocation(shader->program, "shadowmap"), depthMap);
}

void Shadow::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
/*
void renderShadow(Shader *shader, GLuint depthFBO) {
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	lightView = glm::lookAt(_lightSource, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	glUniformMatrix4fv(glGetUniformLocation(shader->program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderScene(simpleDepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}*/