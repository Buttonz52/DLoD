#ifndef CAMERA_H
#define CAMERA_H

#include "../Game/Utility.h"
#include "GEO/vehicle/Vehicle.h"

class Camera
{
private:
	float azu;
	float alt;
	float radius;
  
	float fov;
	float _near;
	float _far;

	//float exposure;
  float asp;
	
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

	//void increaseExposure(){exposure *= 1.5f;}
	//void decreaseExposure(){exposure/=1.5f;}
	//float exposure() { return exposure;}

  void setAsp(float);
 
	vec3 focalPoint;
	vec3 position;
	mat4 calculateProjectionMatrix();
	mat4 calculateViewMatrix();

	void translate3D(vec3);
	vec3 *getCenter();
	void setInitValues();

	void followVehicle(Vehicle*);
};


#endif