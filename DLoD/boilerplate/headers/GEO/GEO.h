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
	string filename;

	bool init();
	void shutdown();		//destroy shader, texture, mesh
	void ReadObjects(const string &filename);

private:
	vec3 position;
	double radius;
	Mesh mesh;
	Shader shader;
	Texture texture;


	

};
#endif