
#include "headers/Camera.h"


std::string str(glm::vec3 v)
{ return "" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z); }


using namespace std;

Camera::Camera()
{
  azu = 0;
  alt = M_PI_4;
  radius = 20;
  
  fov = M_PI/3;
  _near = 0.001;
  _far = 100;
  
  focalPoint = glm::vec3(0,0,0);
}

Camera::~Camera() {
}
glm::mat4 Camera::calculateProjectionMatrix(float asp) {

  float dy = _near * tan(fov * 0.5);
  float dx = dy * asp;
  
  glm::mat4 projection = glm::mat4(1);
  
  projection[0][0] = _near / dx;
  projection[1][1] = _near / dy;
  projection[2][2] = -(_far + _near)/(_far - _near);
  projection[2][3] = -1;
  projection[3][2] = (2 * _far * _near) / (_far - _near);
  projection[3][3] = 1;
  
  return projection;
}


glm::mat4 Camera::calculateViewMatrix(glm::vec3 center) {
  
  // Calculate view matrix

  focalPoint = center;
  
  glm::mat4 camera = glm::mat4();
  
  float x = radius * sin(alt) * cos(azu);
  float y = radius * cos(alt);
  float z = radius * sin(alt) * sin(azu);
  
  //  cout << "x: " << x << " y: " << y << " z: " << z << endl;
  
  glm::vec3 eye = glm::vec3(x,y,z);
  glm::vec3 look = -glm::normalize(eye);
  glm::vec3 up = glm::vec3(0,1,0);
  glm::vec3 n = glm::normalize(look);
  glm::vec3 u = glm::normalize(glm::cross(look, up));
  glm::vec3 v = glm::cross(u,n);
  
  camera[0][0] = u.x;
  camera[1][0] = u.y;
  camera[2][0] = u.z;
  camera[3][0] = 0;

  camera[0][1] = v.x;
  camera[1][1] = v.y;
  camera[2][1] = v.z;
  camera[3][1] = 0;

  camera[0][2] = -n.x;
  camera[1][2] = -n.y;
  camera[2][2] = -n.z;
  camera[3][2] = 0;

  camera[0][3] = 0;
  camera[1][3] = 0;
  camera[2][3] = 0;
  camera[3][3] = 1;
  
  glm::mat4 translation = glm::mat4(1);
  translation[3][0] = -eye.x - focalPoint.x;
  translation[3][1] = -eye.y - focalPoint.y;
  translation[3][2] = -eye.z - focalPoint.z;

  return camera * translation;
}