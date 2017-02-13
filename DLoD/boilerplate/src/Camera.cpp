#include "GEO\Camera.h"

Camera::Camera()
{
	setInitValues();
	center = vec3(0);
	focalPoint = glm::vec3(0, 0, 0);
}

Camera::Camera(vec3 &c) {
	setInitValues();
	focalPoint = glm::vec3(0, 0, 0);
	center = c;
}
Camera::Camera(vec3 &c, vec3 &f) {
	setInitValues();
	center = c;
	focalPoint = f;
}
Camera::~Camera() {}

void Camera::setInitValues() {
	azu = M_PI_4;
	alt = M_PI_4;
	radius = 100;

	fov = M_PI / 3;
	_near = 0.001;
	_far = 600;
}

void Camera::setCenter(vec3 &c) {
	center = c;
}
void Camera::setAlt(float newAlt)
{
	alt = newAlt;
	alt = min(max(alt, 0.000001f), M_PI - 0.000001f);
}

void Camera::setAzu(float newAzu)
{
	azu = newAzu;
}

void Camera::incrementAlt(float newAlt)
{
	alt += newAlt;
	alt = min(max(alt, 0.001f), M_PI - 0.001f);
}

void Camera::incrementAzu(float newAzu)
{
	azu += newAzu;
}

void Camera::setRadius(float newRad)
{
	radius = newRad;
}

void Camera::incrementRadius(float newRad)
{
	radius -= newRad;
	radius = min(max(radius, 6.0f), 200.0f);
}

mat4 Camera::calculateProjectionMatrix(float asp) 
{
	float dy = _near * tan(fov * 0.5);
	float dx = dy * asp;
  
	mat4 proj = perspective(fov, asp, _near, _far);

	return proj;
}

void Camera::translate3D(vec3 delta)
{
	vec4 temp = vec4(focalPoint,0);
	temp = temp + vec4(delta,0) * calculateViewMatrix();
  
	focalPoint = vec3(temp);
}

mat4 Camera::calculateViewMatrix()
{
	// Calculate x,y,z from spherical coordinates
	float x = radius * sin(alt) * cos(azu - M_PI / 2);
	float y = radius * cos(alt);
	float z = radius * sin(alt) * sin(azu - M_PI / 2);
	//  cout << "x: " << x << " y: " << y << " z: " << z << endl;
	//focalPoint = c;
	vec3 eye(x, y, z);
	vec3 up(0.0f, 1.0f, 0.0f);
	//vec3 center(0.0f, 0.0f, 0.0f);

	mat4 view = lookAt(eye, center, up);
  
	view = translate(view, focalPoint);
	
	return view;
}

vec3* Camera::getCenter() 
{
	return &center;
}

void Camera::followVehicle(Vehicle* v)
{
	mat4 M = v->getModelMatrix();
	
	// Get the position of the object
	vec3 pos = vec3(M[3]);
	
	// Make a delta value to increment the focal point by  (This should evtually be a function of the length of delta)
	vec3 delta = -pos - focalPoint;
	delta = (delta.length() > 0.01) ? vec3(delta.x / 10, delta.y / 10, delta.z / 10) : delta;
	
	focalPoint += delta;
	
	// Get the rotation of the object
	physx::PxVec3 axis = physx::PxVec3(0, 1, 0);
	physx::PxReal angle = 0;
	v->physXVehicle->getRigidDynamicActor()->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);
	
		
	int fix = (axis.y < 0) ? 1 : -1;
	angle *= fix;
	if (angle < 0)
		angle += M_PI * 2;
	
	if (angle - azu > M_PI * 2)
		azu -= M_PI * 2;
	
	// Make a delta value to increment the azu by  (This should evtually be a function of the length of the delta)
	double theta = angle - azu;
	if (theta > M_PI)
		theta -= M_PI * 2;
	
	if (theta < -M_PI)
		theta += M_PI * 2;
	
	theta = (abs(theta) > 0.001) ? theta / 20 : theta;
	
	azu += theta;
	if (azu < 0)
		azu += M_PI * 2;
	
	if (azu > M_PI * 2)
		azu -= M_PI * 2;
} 		  

