#include "headers/Utility.h"

class Camera
{
private:
	float azu;
	float alt;
	float radius;
  
	float fov;
	float _near;
	float _far;
 
public:
	Camera();

	void setAzu(float);
	void setAlt(float);
	void setRadius(float);
  
	void incrementAzu(float);
	void incrementAlt(float);
	void incrementRadius(float);
 
	vec3 focalPoint;
	mat4 calculateProjectionMatrix(float asp);
	mat4 calculateViewMatrix();

	void translate3D(vec3);
};