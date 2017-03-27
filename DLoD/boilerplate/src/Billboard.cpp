#include "Game\Billboard.h"



Billboard::Billboard()
{
	hasTexture = 0;
	mixColour = 0;
	mixAmount = 0.5f;

	rotateZ = 0;
	transparency = 1.f;

	vertexArray = -1;
	vertexBuffer = -1;
	textureBuffer = -1;
	colourBuffer = -1;
	elementCount = 0;

	_scale = vec3(1);
	position = vec3(0);
	colour = vec3(0);
}


Billboard::~Billboard()
{
	Destroy();
}

void Billboard::setPosition(const vec3 &p) {
	position = p;
}

vec3 & Billboard::getPosition() {
	return position;
}

void Billboard::setScale(const vec3 &s) {
	_scale = s;
}

vec3 &Billboard::getScale() {
	return _scale;
}


void Billboard::Destroy() {
	mainShader.DestroyShaders();
	mainTexture.DestroyTexture();
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &colourBuffer);

	glDeleteVertexArrays(1, &vertexArray);
}

void Billboard::updateModelMatrix() {
	//Create the model matrix
	mat4 mscale = glm::scale(mat4(), _scale);
	mat4 translate = glm::translate(mat4(), position);
	mat4 rotate = glm::rotate(mat4(), rotateZ, vec3(0, 0, 1));

	modelMatrix = translate*rotate*mscale*mat4();	//translations/rotations
}

//http://nehe.gamedev.net/article/billboarding_how_to/18011/
void Billboard::calculateViewMatrix(const vec3 &camUp, const vec3 &campos) {
	//could/should probably just use glm::lookAt...

	//calculate look vector
	//vec4 worldPos4 = modelMatrix * vec4(position, 1.f);
	//vec3 worldPos = vec3(worldPos4.x, worldPos4.y, worldPos4.z) / worldPos4.w;
	//vec3 worldPos = modelMatrix[3];
	vec3 worldPos = position;
	cout <<"CAMPOS: "<< campos.x << " " << campos.y << " " << campos.z << endl;
	cout << "POS: " << worldPos.x << " " << worldPos.y << " " << worldPos.z << endl;
	vec3 look = Normalize(campos - worldPos);
	look.y = 0;
	//get right vector; use camera as temporary up vector
	vec3 right = Normalize(glm::cross(Normalize(camUp), look));

	//Get up vector for billboard
	vec3 up = Normalize(glm::cross(look, right));

	viewMatrix = mat4(vec4(right,0), vec4(up,0), vec4(look,0), vec4(worldPos,1));
}

//Normalizes a vec3
vec3 Billboard::Normalize(const vec3 &v) {
	float magn = v.x*v.x + v.y*v.y + v.z*v.z;
	if (magn <= 0) {
		return vec3(0);
	}

	return (v / sqrt(magn));
}
//init shaders
void Billboard::InitializeShaders(const string & vert, const string & frag)
{
	mainShader.program = mainShader.InitializeShaders(vert, frag);
}

//init texture
bool Billboard::initTexture(const string &filename, GLuint target) {
	hasTexture = 1;
	return mainTexture.InitializeTexture(filename, target);
}


//makes a square/rectangle depending on scale
bool Billboard::GenerateSquareVertices(float scale_x, float scale_y, const vec3 &col) {
	//uvs.clear();
	//vertices.clear();
	colours.clear();
	colour = col;
	vertices = { vec3(-1 * scale_x,-1 * scale_y,0),
		vec3(-1 * scale_x,1 * scale_y,0),
		vec3(1 * scale_x,-1 * scale_y,0),
		vec3(1 * scale_x,1 * scale_y,0)
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
	elementCount = vertices.size();
	for (int i = 0; i < elementCount; i++)
		colours.push_back(col);
	return Initialize();
}

//makes a shape via user generated vertices, uv coordinates, and colours.
bool Billboard::GenerateVertices(const vector<vec3> *verts, const vec3 &col, const vector<vec2> *uv) {
	colours.clear();
	vertices = *verts;
	uvs = *uv;
	colour = col;
	for (int i = 0; i < vertices.size(); i++)
		colours.push_back(col);
	elementCount = vertices.size();
	return Initialize();
}


bool Billboard::Initialize() {

	/* Initialization of buffers for mesh goes here */

	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint UV_INDEX = 1;
	const GLuint COLOUR_INDEX = 2;

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_INDEX);

	//}
	// create another one for storing our uv
	if (uvs.size() != 0) {
		glGenBuffers(1, &textureBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), uvs.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(UV_INDEX);
	}

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(vec3), colours.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(COLOUR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(COLOUR_INDEX);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

void Billboard::Render(GLuint type, const mat4 & projMatrix, const vec3 &camUp, const vec3 &campos)
{
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(mainShader.program);
	glBindVertexArray(vertexArray);

	if (type == GL_POINTS) {
		glPointSize(10.f);
	}
	if (hasTexture == 1) {
		mainTexture.BindTexture(mainShader.program, "sampler");
	}
	//updateModelMatrix();	
	//calculateViewMatrix(camUp, campos);
	glUniformMatrix4fv(glGetUniformLocation(mainShader.program, "model"), 1, GL_FALSE, value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(mainShader.program, "view"), 1, GL_FALSE, value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(mainShader.program, "projection"), 1, GL_FALSE, value_ptr(projMatrix));

	glUniform1i(glGetUniformLocation(mainShader.program, "hasTexture"), hasTexture);
	glUniform1i(glGetUniformLocation(mainShader.program, "mixColour"), mixColour);
	glUniform1f(glGetUniformLocation(mainShader.program, "transparency"), transparency);
	glUniform1f(glGetUniformLocation(mainShader.program, "mixAmount"), mixAmount);

	glDrawArrays(type, 0, elementCount);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	if (hasTexture)
		mainTexture.UnbindTexture();
	// check for an report any OpenGL errors
	CheckGLErrors();
}
