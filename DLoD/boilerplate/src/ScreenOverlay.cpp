/* Class for loading screen/projection stuff.
*/


#include "..\headers\Game\ScreenOverlay.h"



ScreenOverlay::ScreenOverlay()
{

	vertexArray = -1;
	elementCount = 0;

	vertexBuffer = -1;
	textureBuffer = -1;
	colourBuffer = -1; //might not need
	colour = vec3(0);
	hasTexture = 0;
}


ScreenOverlay::~ScreenOverlay()
{
	Destroy();
}

//init shaders
void ScreenOverlay::InitializeShaders(const string & vert, const string & frag)
{
	shader.program = shader.InitializeShaders(vert, frag);
}

//makes a square/rectangle depending on scale
bool ScreenOverlay::GenerateSquareVertices(const float scale_x, const float scale_y) {
	uvs.clear();
	vertices.clear();
	colours.clear();
	vertices = {vec3(-1*scale_x,-1 * scale_y,0),
		vec3(-1 * scale_x,1 * scale_y,0),
		vec3(1 * scale_x,-1*scale_y,0),
		vec3(1 * scale_x,1*scale_y,0)
	};
	//if (hasTexture ==1) {
		/*uvs = {vec2(0,0),
		vec2(0,1),
		vec2(1,0),
		vec2(1,1)};*/	
	uvs = { vec2(0,1),
		vec2(0,0),
		vec2(1,1),
		vec2(1,0)
		
	};

//	}
//	else {
		for (int i = 0; i < 4; i++) {
			colours.push_back(colour);
	//	}
	}
		elementCount = vertices.size();
	return Initialize();
}

//makes a shape via user generated vertices, uv coordinates, and colours.
bool ScreenOverlay::GenerateVertices(const vector<vec3> *verts, const vector<vec3> *colour, const vector<vec2> *uv) {
	vertices = *verts;
	uvs = *uv;
	colours = *colour;
	elementCount = vertices.size();
	return Initialize();
}


//init texture
bool ScreenOverlay::initTexture(const string &filename, GLuint target) {
	hasTexture = 1;
	return texture.InitializeTexture(filename, target);
}


//initialize buffers
bool ScreenOverlay::Initialize() {

	/* Initialization of buffers for mesh goes here */

	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;
	const GLuint UV_INDEX = 2;

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_INDEX);

	// check if has a texture, otherwise pass colour buffer?
	//if (colours.size() != 0) {
		// create another one for storing our colours
		glGenBuffers(1, &colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(vec3), colours.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_INDEX);
	//}
	// create another one for storing our uv
	//if (uvs.size() != 0) {
		glGenBuffers(1, &textureBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), uvs.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(UV_INDEX);
	//}

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

//renders screen.  No model matrix because don't need.
void ScreenOverlay::Render(GLuint type)
{
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader.program);
	glBindVertexArray(vertexArray);

//	if (hasTexture ==1) {
		texture.BindTexture(shader.program, "sampler");
	//}

	vec3 fp = vec3(0, 0, 0);		//focal point
	glUniform1i(glGetUniformLocation(shader.program, "hasTexture"), hasTexture);
	glDrawArrays(type, 0, elementCount);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	if (hasTexture ==1)
		texture.UnbindTexture(GL_TEXTURE_2D);
	// check for an report any OpenGL errors
	CheckGLErrors();
}
void ScreenOverlay::Destroy() {
	shader.DestroyShaders();
	texture.DestroyTexture();
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &colourBuffer);

	glDeleteVertexArrays(1, &vertexArray);
}
