#include "GEO\GEO.h"

GEO::GEO()
{
	position = vec3(0);
	radius = 1.f;
	filename = "";
	scale = vec3(1.f);
	rotation = vec3(0);
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
		scale -=s;
	}
 }
void GEO::setRotation(const vec3 & r)
{
	rotation = r;
}

vec3 &GEO::getRotation()
{
	return rotation;
}

void GEO::updateRotation(const vec3 &r) {
	rotation += r;
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

Shader GEO::getShader()
{
	return shader;
}

void GEO::setShader(const Shader &s)
{
	shader = s;
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
	if (!mesh.Initialize()) {
		return 0;
	}
	return 1;
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