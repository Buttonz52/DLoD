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

	vec3 GetPosition();
	void SetPosition(vec3 pos);
	double GetRadius();
	void setFilename(const string &fname);
	string getFilename();
	void addMeshShader();
	void setMesh(Mesh m);
	Mesh& getMesh();
	Shader getShader();
	void setShader(Shader &s);

	void shutdown();		//destroy shader, texture, mesh

private:
	vec3 position;
	double radius;
	string filename;
	Mesh mesh;
	Shader shader;
	Texture texture;
	string audioFile;
};
#endif