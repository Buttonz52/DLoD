#include "../Game/Utility.h"
#include "GEO.h"

class Camera : GEO
{
private:
	float azu;
	float alt;
	float radius;
  
	float fov;
	float _near;
	float _far;
	
	vec3 center;
public:
	Camera();
	Camera(vec3 &center);
	Camera(vec3 &center, vec3 &focalPoint);
	~Camera();

	void setCenter(vec3 &);

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
	vec3 *getCenter();
	void setInitValues();
};