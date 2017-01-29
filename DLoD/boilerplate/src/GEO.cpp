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

vec3& GEO::getPosition()
{
	return position;
}

void GEO::setPosition(vec3 &pos)
{
	position = pos;
}

double GEO::getRadius()
{
	return 0.0;
}

void GEO::setFilename(const string &fname)
{
	filename = fname;
}

string& GEO::getFilename()
{
	return filename;
}

Mesh& GEO::getMesh()
{
	return mesh;
}

Shader GEO::getShader()
{
	return shader;
}


//Adds mesh file to mesh vector based on directory
void GEO::addMeshShader()
{
	//Get all information for mesh

	if (!mesh.ReadMesh("models/" + filename))
		cout << "Error reading mesh" << endl;

	//Add colour for the moment; this can be taken out
	//or colour changed/colour added to obj files and 
	//not here
	vec3 red(1.f, 0.f, 0.f);
	mesh.AddColour(&red);
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
void GEO::shutdown()
{
	mesh.DestroyMesh();
	shader.DestroyShaders();
}