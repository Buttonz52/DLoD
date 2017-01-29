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

	vec3& getPosition();
	void setPosition(vec3 &pos);
	double getRadius();
	void setFilename(const string &fname);
	string& getFilename();
	void addMeshShader();
	Mesh& getMesh();
	Shader getShader();

	void shutdown();		//destroy shader, texture, mesh

private:
	vec3 position;
	double radius;
	string filename;
	Mesh mesh;
	Shader shader;
	Texture texture;
};
#endif