#ifndef GEO_H
#define GEO_H
#include "..\Game\Utility.h"
#include "..\GameEngine\Mesh.h"
\
//Game Entity Object
class GEO
{
public:
	GEO();
	~GEO();



	vec3 GetPosition();
	void SetPosition(vec3 pos);
	double GetRadius();
	//bool RenderMesh(mat4 winRatio, vec3 lightSource, mat4 view, mat4 proj, int width, int height);
	Mesh mesh;
private:
	vec3 position;
	double radius;

};
#endif