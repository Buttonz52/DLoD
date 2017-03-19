#include "GEO\GEO.h"

GEO::GEO()
{
	position = vec3(0);
	radius = 1.f;
	scale = vec3(1.f);
	xRotation = 0; yRotation = 0; zRotation = 0;
	hasTexture = 0;
	isSkybox = 0;
	colour = vec3(1,1,1);
	transparency = 1.f;
}

GEO::~GEO()
{
  shutdown();
  for (GEO* child : children)
    delete child;
}

void GEO::removeChild(GEO * child)
{
  vector<GEO*>::iterator itr = children.begin();
  while (itr != children.end()) {
    if (*itr._Ptr == child) {
      itr = children.erase(itr);
      break;
    }
    else {
      (*itr._Ptr)->removeChild(child);
      ++itr;
    }
  }
}

double GEO::getRadius()
{
	return radius;
}

vec3 &GEO::getScale()
{
	return scale;
}

void GEO::setScale(const vec3 & s)
{
	scale = s;
}

void GEO::updateScale(const vec3 &s) {
	//ASSUMPTION: scale is equal and no shearing/stretching
	//can update later to include potential stretching and stuff
	scale += s;
	if (scale.x <= 0) {
		scale -= s;
	}
}

// Get the objects total rotation matrix
mat4 GEO::getRotation()
{
	mat4 x = mat4(1);
	mat4 y = mat4(1);
	mat4 z = mat4(1);

	// fix the rotation around the z axis
	z[0][0] = cos(zRotation);
	z[0][1] = -sin(zRotation);
	z[1][0] = sin(zRotation);
	z[1][1] = cos(zRotation);

	// fix the rotation around the y axis
	y[0][0] = cos(yRotation);
	y[0][2] = -sin(yRotation);
	y[2][0] = sin(yRotation);
	y[2][2] = cos(yRotation);

	// fix the rotation around the x axis
	x[1][1] = cos(xRotation);
	x[1][2] = -sin(xRotation);
	x[2][1] = sin(xRotation);
	x[2][2] = cos(xRotation);

	// Return the rotation matrix
	return x * y * z;
}

//set rotation of GEO
void GEO::setRotation(const vec3 & r)
{
	xRotation = r.x;
	yRotation = r.y;
	zRotation = r.z;
}

//updates rotation
void GEO::updateRotation(const vec3 &r) {
	if (r.x != 0) {
		xRotation += r.x;
	}
	if (r.y != 0) {
		yRotation += r.y;
	}
	if (r.z != 0) {
		zRotation += r.z;
	}

}


vec3 &GEO::getPosition()
{
	//return vec3(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
	return position;
}

void GEO::setPosition(const vec3 &pos)
{
	position = pos;
}

void GEO::updatePosition(const vec3 &p) {
	position += p;
}


mat4 GEO::getModelMatrix()
{
	return modelMatrix;
}

void GEO::setModelMatrix(mat4 m)
{
	modelMatrix = m;
}

void GEO::updateModelMatrix()
{
	mat4 s = glm::scale(this->getScale());
	mat4 r = this->getRotation();
	mat4 t = translate(this->getPosition());

	modelMatrix = t * r * s;
}

//Adds shaders 
void GEO::addShaders(const string &vert, const string &frag)
{
	shader.InitializeShaders(vert, frag);
}

bool GEO::initBuffers() {
	//if there's a bump texture, init tangent buffer
	return mesh.Initialize();
}

bool GEO::initMesh(const string &filename) {
	//Get all information for mesh

	if (!mesh.ReadMesh("models/" + filename)) {
		cout << "Error reading mesh" << endl;
		return 0;
	}
	if (!mesh.Initialize()) {
		cout << "Error  with initializiation of mesh"<<endl;
		return 0;
	}

	cout << "number of verts: " << mesh.vertices.size() << endl;
	cout << "Loaded " << filename << endl;
	return 1;
}

bool GEO::initTexture(const string &filename, GLuint target) {
	hasTexture = 1;
	isPlane = 1;
	return texture.InitializeTexture(filename, target);
}

//
//
///** Calculates tangent coordinates for mesh. */
//void GEO::calculateMeshTangent() {
//	mesh.calculateMeshTangent(); 
//}

bool GEO::initSkybox(const vector <string> &filenames) {
	hasTexture = 1;
	isSkybox = 1;
	return texture.InitializeSkybox(filenames);
}

Shader& GEO::getShader()
{
	return shader;
}

void GEO::setShader(const Shader &s)
{
	shader = s;
}

Texture& GEO::getTexture() {
	return texture;
}

void GEO::setTexture(const Texture &tex) {
	texture = tex;
}

Mesh& GEO::getMesh()
{
	return mesh;
}

void GEO::setMesh(const Mesh &m)
{
	mesh = m;
}

void GEO::setColour(const vec3 &col) {
	colour = col;
	mesh.AddColour(col);
}

vec3 *GEO::getColour() {
	return &colour;
}

// Rendering function that draws our scene to the frame buffer
//TODO: Make specific to different types of GEOs, use inheritance
void GEO::Render(const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource)
{

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader.program);
	glBindVertexArray(mesh.vertexArray);

	if (hasTexture) {
		texture.BindTexture(shader.program, "sampler");
	}

	vec3 fp = vec3(0, 0, 0);		//focal point

	mat4 M = getModelMatrix();
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat near_plane = 0.001f, far_plane = 1000.f;
	//lightProjection = glm::ortho(-600.0f, 600.0f, -600.0f, 600.0f, near_plane, far_plane);
	lightProjection = glm::ortho(-1000.f, 1000.f, -1000.f, 1000.f, near_plane, far_plane);

	//lightProjection = glm::perspective(float(M_PI / 3),1920.f/1080.f,near_plane, far_plane);
	lightView = glm::lookAt(_lightSource, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	glUniform1f(glGetUniformLocation(shader.program, "transparency"), transparency);

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, value_ptr(M));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "modelview"), 1, GL_FALSE, value_ptr(_view));

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(shader.program, "lightPosition"), 1, value_ptr(_lightSource));

	glDrawElements(GL_TRIANGLES, mesh.elementCount, GL_UNSIGNED_SHORT, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	if (hasTexture) {
		texture.UnbindTexture();
	}

	// check for an report any OpenGL errors
	CheckGLErrors();

  for (int i = 0; i < children.size(); i++)		//GEO* child : children)
    children[i]->Render(_view, _projection, _lightSource);
}

void GEO::shutdown()
{
	shader.DestroyShaders();
	texture.DestroyTexture();
	mesh.DestroyMesh();

  for (GEO* child : children)
    child->shutdown();
}

void GEO::playSFX(const string &name, const int &volume, const int &channel)
{
	//audio.PlaySfx(sfx);
	audio.PlaySfx(sfxMap[name], volume, channel);
}