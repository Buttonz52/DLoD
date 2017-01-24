#include "GEO\GEO.h"



GEO::GEO()
{
}


GEO::~GEO()
{
}

vec3 GEO::GetPosition()
{
	return vec3();
}

void GEO::SetPosition(vec3 pos)
{
}

double GEO::GetRadius()
{
	return 0.0;
}

/*	//doesn't work yet
bool GEO::RenderMesh(mat4 winRatio, vec3 lightSource, mat4 view, mat4 proj, int width, int height) {
	// Rendering function that draws our scene to the frame buffer
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(mesh.shader.program);
	glBindVertexArray(mesh.vertexArray);

	vec3 fp = vec3(0, 0, 0);		//focal point

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.program, "modelview"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.program, "projection"), 1, GL_FALSE, value_ptr(proj));
	glUniform3fv(glGetUniformLocation(mesh.shader.program, "lightPosition"), 1, value_ptr(lightSource));

	//mesh->texture.BindTexture(shader->program, GL_TEXTURE_2D, "sampler");

	glDrawElements(GL_TRIANGLES, mesh.elementCount, GL_UNSIGNED_SHORT, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	//mesh->texture.UnbindTexture(GL_TEXTURE_2D);
	// check for an report any OpenGL errors
	return !CheckGLErrors();
}*/