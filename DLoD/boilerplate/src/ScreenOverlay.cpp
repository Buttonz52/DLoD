/* Class for loading screen/projection stuff.
*/


#include "..\headers\Game\ScreenOverlay.h"



ScreenOverlay::ScreenOverlay()
{
	isFontTex = 0;
	isRedTransparent = 0;

	hasTexture = 0;
	mixColour = 0;
	mixAmount = 0.5f;

	rotateZ = 0;
	transparency = 1.f;

	vertexArray = -1;
	vertexBuffer = -1;
	textureBuffer = -1;
	elementCount = 0;

	_scale = vec3(1);
	position = vec3(0);
	colour = vec3(0);
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

bool ScreenOverlay::GenerateBorder(float scale_x, float scale_y, const float &thicc, const vec3 &col, const vec3 &position) {
	colour = col;
	this->position = position;
	vertices = { vec3(-1 * scale_x,1 * scale_y,0),			//top bar
				vec3(1*scale_x,(1-thicc)*scale_y,0),
				vec3(-1*scale_x, (1-thicc)*scale_y,0),
				vec3(-1 * scale_x,1 * scale_y,0),
				vec3(1 * scale_x,1 * scale_y,0),
				vec3(1*scale_x,(1-thicc)*scale_y,0),

				vec3(1 * scale_x,(1-thicc) * scale_y,0),			//side right
				vec3(1 * scale_x,(-1 + thicc)*scale_y,0),
				vec3((1 - thicc) *scale_x, (1 - thicc)*scale_y,0),
				vec3((1 - thicc) *scale_x, (1 - thicc)*scale_y,0),
				vec3(1 * scale_x,(-1 + thicc)*scale_y,0),
				vec3((1-thicc) * scale_x,(-1 + thicc)*scale_y,0),

				vec3(-1 * scale_x,(-1+thicc) * scale_y,0),			//buttom bar
				vec3(1 * scale_x,-1*scale_y,0),
				vec3(-1 *scale_x, -1*scale_y,0),
				vec3(-1 * scale_x,(-1 + thicc) * scale_y,0),			
				vec3(1 * scale_x,(-1+thicc) * scale_y,0),
				vec3(1 * scale_x, -1 * scale_y,0),

				vec3(-1 * scale_x,(1 - thicc) * scale_y,0),			//side left
				vec3((-1+thicc) * scale_x,(1 - thicc)*scale_y,0),
				vec3((-1 +thicc) *scale_x, (-1 + thicc)*scale_y,0),
				vec3(-1 * scale_x,(1 - thicc) * scale_y,0),	
				vec3((-1 + thicc) *scale_x, (-1 + thicc)*scale_y,0),
				vec3(-1 * scale_x,(-1 + thicc)*scale_y,0),		
	};

	uvs = { vec2(0,1),			//top 
			vec2(1,1-thicc),
			vec2(0,1-thicc),
			vec2(0,1),
			vec2(1,0),
			vec2(0,1 - thicc),

			vec2(1-thicc, thicc),		//side right
			vec2(1,1-thicc),
			vec2(1-thicc, 1-thicc),
			vec2(1 - thicc, thicc),
			vec2(1, thicc),
			vec2(1 - thicc, 1 - thicc),

			vec2(1,1-thicc),		//bottom 
			vec2(1,1),
			vec2(0,1),
			vec2(1,1 - thicc),
			vec2(1,1-thicc),
			vec2(1,1),

			vec2(0,thicc),			//left
			vec2(thicc,1-thicc),
			vec2(0,1-thicc),
			vec2(0,thicc),
			vec2(thicc, thicc),
			vec2(thicc,1 - thicc)

	};
	elementCount = vertices.size();
	return Initialize();
}
//makes a square/rectangle depending on scale
bool ScreenOverlay::GenerateSquareVertices(float scale_x, float scale_y, const vec3 &col) {
	//uvs.clear();
	//vertices.clear();
	colour = col;
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
	elementCount = vertices.size();
	return Initialize();
}

//makes a shape via user generated vertices, uv coordinates, and colours.
bool ScreenOverlay::GenerateVertices(const vector<vec3> *verts, const vec3 &col, const vector<vec2> *uv) {
	vertices = *verts;
	uvs = *uv;
	colour = col;
	elementCount = vertices.size();
	return Initialize();
}


//init texture
bool ScreenOverlay::initTexture(const string &filename, GLuint target) {
	hasTexture = 1;
	return texture.InitializeTexture(filename, target);
}

void ScreenOverlay::setColour(const vec3 &c) {
	colour = c;
}

vec3 & ScreenOverlay::getColour() {
	return colour;
}
//initialize buffers
void ScreenOverlay::UpdateBuffers(const vector<vec2> *uvs) {
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	//glBufferData(GL_ARRAY_BUFFER, uvs->size() * sizeof(vec2), uvs->data(), GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uvs->size() * sizeof(vec2), uvs->data());
	//const GLuint UV_INDEX = 1;

	//glGenBuffers(1, &textureBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	//glBufferData(GL_ARRAY_BUFFER, uvs->size() * sizeof(vec2), uvs->data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(UV_INDEX);
}

void ScreenOverlay::UpdateVertices(const vector<vec3> *vertices) {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices->size() * sizeof(vec3), vertices->data());
	elementCount = vertices->size();
}
bool ScreenOverlay::Initialize() {

	/* Initialization of buffers for mesh goes here */

	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint UV_INDEX = 1;

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

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

//renders screen.  No model matrix because don't need.
void ScreenOverlay::Render(GLuint type, const vec3 &colour)
{
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader.program);
	glBindVertexArray(vertexArray);

	if (type == GL_POINTS) {
		glPointSize(10.f);
	}
	if (hasTexture ==1) {
		texture.BindTexture(shader.program, "sampler");
	}
		//Create the model matrix
	mat4 mscale = scale(mat4(), _scale);
		mat4 translate = glm::translate(mat4(), position);//Normalize(position));
	mat4 rotate = glm::rotate(mat4(), rotateZ, vec3(0,0,1));

	mat4 model = translate*rotate*mscale*mat4();	//translations/rotations
	vec3 fp = vec3(0, 0, 0);		//focal point
	glUniform3fv(glGetUniformLocation(shader.program, "colour"), 1, value_ptr(colour));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE,value_ptr(model));
	glUniform1i(glGetUniformLocation(shader.program, "hasTexture"), hasTexture);
	glUniform1i(glGetUniformLocation(shader.program, "mixColour"), mixColour);
	glUniform1i(glGetUniformLocation(shader.program, "isFontTex"), isFontTex);
	glUniform1i(glGetUniformLocation(shader.program, "isRedTransparent"), isRedTransparent);
	glUniform1f(glGetUniformLocation(shader.program, "transparency"), transparency);
	glUniform1f(glGetUniformLocation(shader.program, "mixAmount"), mixAmount);



	glDrawArrays(type, 0, elementCount);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	if (hasTexture)
		texture.UnbindTexture();
	// check for an report any OpenGL errors
	CheckGLErrors();
}

//Sets position of overlay
void ScreenOverlay::setPosition(const vec3 &pos) {
	position = pos;

	//Check that in bounds, clamp if not.
	if (position.x > 1.f) {
		position.x = 1.f;
	}
	if (position.x < -1.f) {
		position.x = -1.f;
	}
	if (position.y > 1.f) {
		position.y = 1.f;
	}
	if (position.y < -1.f) {
		position.y = -1.f;
	}
}

vec3 & ScreenOverlay::getPosition() {
	return position;
}
//rotate overlay in Z direction
void ScreenOverlay::setRotateZ(const float &r) {
	rotateZ = r;
}

void ScreenOverlay::setScale(const vec3 &s) {
	_scale = s;
}
vec3 & ScreenOverlay::getScale() {
	return _scale;
}
void ScreenOverlay::setHasTexture(const bool &b) {
	hasTexture = b;
}

int ScreenOverlay::getHasTexture() {
	return hasTexture;
}


void ScreenOverlay::setMixFlag(const bool &b) {
	mixColour = b;
}

int ScreenOverlay::getMixFlag() {
	return mixColour;
}

void ScreenOverlay::setMixAmount(const float &c) {
	mixAmount = c;
}
float ScreenOverlay::getMixAmount() {
	return mixAmount;
}


void ScreenOverlay::setTransparency(const float &t) {
	transparency = t;
}
float ScreenOverlay::getTransparency() {
	return transparency;
}
void ScreenOverlay::Destroy() {
	shader.DestroyShaders();
	texture.DestroyTexture();
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &textureBuffer);

	glDeleteVertexArrays(1, &vertexArray);
}

//Normalizes a vec3
vec3 ScreenOverlay::Normalize(const vec3 &v) {
	float magn = v.x*v.x + v.y*v.y + v.z*v.z;
	if (magn <= 0) {
		return vec3(0);
	}

	return (v / sqrt(magn));
}

//something about this is making the scale all weird for loading overlays
bool ScreenOverlay::InitQuad(const string &tex, 
	const string &vert, 
	const string &frag, 
	float x_scale, 
	float y_scale,
	const vec3 &colour) 
{
	if (!strcmp(tex.c_str(),"")) {
		if (!initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
			cout << "Failed to init loadBkgrnd." << endl;
			return false;
		}
	}

#if DEBUG
	cout << "Init loadBkgrd shaders ";
#endif
	InitializeShaders(vert, frag);
#if DEBUG
	cout << " -initialized";
#endif
	if (!GenerateSquareVertices(x_scale,y_scale,colour)) {
		cout << "Failed to initialize screen overlay." << endl;
		return false;
	}
	//setPosition(position);

	return true;
}


void ScreenOverlay::InitializeGameText(const string &text, const vec3 &position, const vec3 &colour, int kerning) {
	isFontTex = 1;
	setColour(colour);
	if (!initTexture("fonts/grim12x12.png", GL_TEXTURE_2D)) {
		cout << "Failed to init fonts." << endl;
	}
	InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	vector<vec3> verts;
	vector<vec2> uvs;

	int index = 0;
	for (char c : text) {
		GenerateTextUVs(uvs, c);

		verts.push_back(vec3(0 + float(index) / kerning, 0, 0));
		verts.push_back(vec3(0.05 + float(index) / kerning, 0, 0));
		verts.push_back(vec3(0 + float(index) / kerning, 0.1, 0));
		verts.push_back(vec3(0.05 + float(index) / kerning, 0, 0));
		verts.push_back(vec3(0 + float(index) / kerning, 0.1, 0));
		verts.push_back(vec3(0.05 + float(index) / kerning, 0.1, 0));
		index++;
	}

	//if (!fontTex->GenerateSquareVertices(0.1, 0.1, vec3(0))) {
	if (!GenerateVertices(&verts, colour, &uvs)) {
		cout << "Failed to initialize font overlay." << endl;
	}

	setPosition(position);
}
void ScreenOverlay::UpdateGameText(const string &text) {
	vector<vec2> uvs;
	for (char c : text) {
		GenerateTextUVs(uvs, c);
	}
	UpdateBuffers(&uvs);
}

void ScreenOverlay::GenerateTextUVs(vector <vec2> &uvs, const char &ch) {
	//for letters 
	char c = ch;
	float horizontalLoc = int(c) % 16;
	float verticalLoc = int(int(c) / 16);

	uvs.push_back(vec2((horizontalLoc) / 16.f, (verticalLoc + 1.f) / 16.f));	//bottom left
	uvs.push_back(vec2((horizontalLoc + 1.f) / 16.f, (verticalLoc + 1.f) / 16.f));		//bottom right
	uvs.push_back(vec2((horizontalLoc) / 16.f, (verticalLoc) / 16.f));			//top left
	uvs.push_back(vec2((horizontalLoc + 1.f) / 16.f, (verticalLoc + 1.f) / 16.f));
	uvs.push_back(vec2((horizontalLoc) / 16.f, (verticalLoc) / 16.f));
	uvs.push_back(vec2((horizontalLoc + 1.f) / 16.f, (verticalLoc) / 16.f));
}
