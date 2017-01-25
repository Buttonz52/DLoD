#ifndef PLANE_H
#define PLANE_H
#include "GEO.h"
#include "Camera.h"
class Plane :
	public GEO
{
public:
	Plane();
	~Plane();
	Camera cam;
	bool CreatePlane(float size, vec3 *colour, int orientation);
	bool RenderPlane(mat4 winRatio, vec3 lightSource, int width, int height);
};
#endif
