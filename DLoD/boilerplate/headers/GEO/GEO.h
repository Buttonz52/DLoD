#ifndef GEO_H
#define GEO_H
#include "..\Game\Utility.h"
#include "..\GameEngine\Mesh.h"
#include "..\GameEngine\Shader.h"
#include "..\GameEngine\Texture.h"
//Game Entity Object
class GEO
{
public:
	GEO();
	~GEO();
	
	void setScale(const vec3 &scale);
	vec3 &getScale();
	void setRotation(const vec3 &rotation);
	vec3 &getRotation(); 
	vec3 &getPosition();
	void SetPosition(const vec3 &pos);
	double GetRadius();
	void setFilename(const string &fname);
	string getFilename();
	//void addMeshShader();
	void addShaders(const string &vertex, const string &fragment);
	void setMesh(const Mesh &m);
	void setColour(const vec3 &col);
	
	Mesh& getMesh();
	Shader getShader();
	void setShader(const Shader &s);
	bool initMesh();
	bool initBuffers();
	void shutdown();		//destroy shader, texture, mesh

private:
	vec3 scale;
	vec3 rotation;
	vec3 position;
	double radius;
	string filename;
	Mesh mesh;
	Shader shader;
	Texture texture;
	string audioFile;
};
#endif