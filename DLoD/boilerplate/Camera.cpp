#include "headers/Camera.h"

Camera::Camera()
{
	azu = M_PI_4;
	alt = M_PI_4;
	radius = 15;
  
	fov = M_PI/3;
	_near = 0.001;
	_far = 100;
  
	focalPoint = glm::vec3(0,0,0);
}

void Camera::setAlt(float newAlt)
{
	alt = newAlt;
	alt = min(max(alt, 0.00001f), M_PI - 0.000001f);
}

void Camera::setAzu(float newAzu)
{
	azu = newAzu;
}

void Camera::incrementAlt(float newAlt)
{
	alt += newAlt;
	alt = min(max(alt, 0.00001f), M_PI - 0.000001f);
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
	radius = min(max(radius, 6.0f), 80.0f);
}

mat4 Camera::calculateProjectionMatrix(float asp) {

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

mat4 Camera::calculateViewMatrix() {
  	// Calculate x,y,z from spherical coordinates
	float x = radius * sin(alt) * cos(azu);
	float y = radius * cos(alt);
	float z = radius * sin(alt) * sin(azu);
	//  cout << "x: " << x << " y: " << y << " z: " << z << endl;

	vec3 eye(x, y, z);
	vec3 up(0.0f, 1.0f, 0.0f);
	vec3 center(0.0f, 0.0f, 0.0f);

	mat4 view = lookAt(eye, center, up);
  
	vec3 translateFromFocal = vec3(focalPoint.x, focalPoint.y, focalPoint.z);
	view = translate(view, translateFromFocal);
	
	return view;
}