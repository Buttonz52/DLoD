#include "GEO\GEO.h"

GEO::GEO()
{
	position = vec3(0);
	radius = 1.f;
	filename = "";
	scale = vec3(1.f);
	xRotation = 0; yRotation = 0; zRotation = 0;
	hasTexture = 0;
	isSkybox = 0;	//make skybox a separate GEO?
}


GEO::~GEO()
{
}

void GEO::setScale(const vec3 & s)
{
	scale = s;
}

vec3 &GEO::getScale()
{
	return scale;
}
void GEO::updateScale(const vec3 &s) {
	//ASSUMPTION: scale is equal and no shearing/stretching
	//can update later to include potential stretching and stuff
	scale += s;
	if (scale.x <= 0) {
		scale -= s;
	}
}

//set rotation of GEO
void GEO::setRotation(const vec3 & r)
{
	xRotation = r.x;
	yRotation = r.y;
	zRotation = r.z;
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
	return position;
}

void GEO::setPosition(const vec3 &pos)
{
	position = pos;
}

void GEO::updatePosition(const vec3 &p) {
	position += p;
}

double GEO::GetRadius()
{
	return 0.0;
}

void GEO::setFilename(const string &fname)
{
	filename = fname;
}

string GEO::getFilename()
{
	return filename;
}

void GEO::setMesh(const Mesh &m)
{
	mesh = m;
}

void GEO::setColour(const vec3 &col) {
	mesh.AddColour(col);
}

Mesh& GEO::getMesh()
{
	return mesh;
}

Shader& GEO::getShader()
{
	return shader;
}

void GEO::setShader(const Shader &s)
{
	shader = s;
}

void GEO::setTexture(const Texture &tex) {
	texture = tex;
}
Texture & GEO::getTexture() {
	return texture;
}
bool GEO::initMesh() {
	//Get all information for mesh

	if (!mesh.ReadMesh("models/" + filename)) {
		cout << "Error reading mesh" << endl;
		return 0;
	}
	if (!mesh.Initialize()) {
		return 0;
	}

	cout << "number of verts: " << mesh.vertices.size() << endl;
	cout << "Loaded " << filename << endl;
	return 1;
}

bool GEO::initBuffers() {
	return mesh.Initialize();

}

bool GEO::initTexture(const string &filename, GLuint target) {
	hasTexture = 1;
	isPlane = 1;
	return texture.InitializeTexture(filename, target);
}
bool GEO::initSkybox(const vector <string> &filenames) {
	hasTexture = 1;
	isSkybox = 1;
	return texture.InitializeSkybox(filenames);
}
//Adds mesh file to mesh vector based on directory
//NOTE: This is kind of overkill for what we actually need.
/*
void GEO::addMeshShader()
{

	string vertex;
	string frag;

	size_t endpos = string(getFilename()).find(".obj");
	string shadername = string(getFilename()).substr(0, endpos);

	string shaderpath = "shaders/";
	//vertex = shaderpath + shadername + ".vert";			<- if each object has a shaderfile
	//frag = shaderpath + shadername + ".frag";
	vertex = shaderpath + "teapot.vert";
	frag = shaderpath + "teapot.frag";

	shader.program = getShader().InitializeShaders(vertex, frag);

	cout << "number of verts: " << mesh.vertices.size() << endl;
	cout << "Loaded " << getFilename() << endl;
}
*/

//Adds shaders 
void GEO::addShaders(const string &vert, const string &frag)
{
	shader.program = getShader().InitializeShaders(vert, frag);
}
void GEO::shutdown()
{
	shader.DestroyShaders();
	texture.DestroyTexture();
	mesh.DestroyMesh();
}