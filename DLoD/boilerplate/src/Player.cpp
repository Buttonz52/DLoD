#include "GEO\player\Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::GetInput()
{
}

bool Player::RenderMesh(mat4 winRatio, vec3 lightSource, int width, int height) {
	// Rendering function that draws our scene to the frame buffer
		// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(vehicle.mesh.shader.program);
	glBindVertexArray(vehicle.mesh.vertexArray);

	vec3 fp = vec3(0, 0, 0);		//focal point

	mat4 _projection = winRatio * playerCam.calculateProjectionMatrix((float)width / (float)height);
	mat4 _view = playerCam.calculateViewMatrix();

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(vehicle.mesh.shader.program, "modelview"), 1, GL_FALSE, value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(vehicle.mesh.shader.program, "projection"), 1, GL_FALSE, value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(vehicle.mesh.shader.program, "lightPosition"), 1, value_ptr(lightSource));

	vehicle.mesh.texture.BindTexture(vehicle.mesh.shader.program, GL_TEXTURE_2D, "sampler");

	glDrawElements(GL_TRIANGLES, vehicle.mesh.elementCount, GL_UNSIGNED_SHORT, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	//mesh->texture.UnbindTexture(GL_TEXTURE_2D);
	// check for an report any OpenGL errors
	return !CheckGLErrors();
}
