#include "..\headers\GEO\Plane.h"



Plane::Plane()
{
}


Plane::~Plane()
{
}

//Creates plane coordinates based on size, colour of plane, and plane orientation.
bool Plane::CreatePlane(float size, vec3 *colour, int orientation) {
	//Plane on z axis
	vec3 x1(-1.f, 1.f, 0.f);
	vec3 x2(-1.f, -1.f, 0.f);
	vec3 x3(1.f, 1.f, 0.f);
	vec3 x4(1.f, -1.f, 0.f);

	//plane on y axis
	vec3 y1(-1.f, 0.f, 1.f);
	vec3 y2(-1.f, 0.f, -1.f);
	vec3 y3(1.f, 0.f, 1.f);
	vec3 y4(1.f, 0.f, -1.f);

	//plane on x axis
	vec3 z1(0.f, -1.f, 1.f);
	vec3 z2(0.f, 1.f, 1.f);
	vec3 z3(0.f, -1.f, -1.f);
	vec3 z4(0.f, 1.f, -1.f);

	vector<vec3> vertices;
	switch (orientation) {
	case 0:
		vertices = { x1*size,x2*size,x3*size,x4*size };
		break;
	case 1:
		vertices = { y1*size,y2*size,y3*size,y4*size };
		break;
	case 2:
		vertices = { z1*size,z2*size,z3*size,z4*size };
		break;
	default:
		vertices = { x1*size,x2*size,x3*size,x4*size };
	}

	//uv coordinates for texturing

	vector <vec3> uvs = { vec3(0.0, 1.0, 0.0),
						vec3(0.0, 0.0, 0.0), 
						vec3(1.0, 1.0, 0.0),
						vec3(1.0, 0.0, 0.0), };
	
	//normal
	vec3 n = cross((vertices[3]-vertices[1]), (vertices[2] - vertices[1]));

	vector<vec3> normals = { n,n,n,n };

	vector<GLushort> faces = {};	//don't actually need

	//only need colour if using colour, not uv
	vector<vec3> colours = {*colour, *colour, *colour, *colour};


	mesh.vertices = vertices;
	mesh.uvs = uvs;
	mesh.normals = normals;
	mesh.faces = faces;
	mesh.colours = colours;

	//initialize plane mesh
	if (!mesh.Initialize()) {
		return 0;
	}
	return 1;
}

//render plane to scene
bool Plane::RenderPlane(mat4 winRatio, vec3 lightSource, int width, int height) {
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

	mat4 _projection = winRatio * cam.calculateProjectionMatrix((float)width / (float)height);
	mat4 _view = cam.calculateViewMatrix();

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.program, "modelview"), 1, GL_FALSE, value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.program, "projection"), 1, GL_FALSE, value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(mesh.shader.program, "lightPosition"), 1, value_ptr(lightSource));
	glUniform3fv(glGetUniformLocation(mesh.shader.program, "position"), 1, value_ptr(GetPosition()));
	glUniform1f(glGetUniformLocation(mesh.shader.program, "scale"), mesh.scale);

	mesh.texture.BindTexture(mesh.shader.program, GL_TEXTURE_2D, "sampler");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	//mesh->texture.UnbindTexture(GL_TEXTURE_2D);
	// check for an report any OpenGL errors
	return !CheckGLErrors();
}
