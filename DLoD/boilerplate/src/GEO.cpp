#include "GEO\GEO.h"

GEO::GEO()
{
	position = vec3(0);
	radius = 1.f;
	filename = "";
}


GEO::~GEO()
{
}

vec3 GEO::GetPosition()
{
	return position;
}

void GEO::SetPosition(const vec3 &pos)
{
	position = pos;
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