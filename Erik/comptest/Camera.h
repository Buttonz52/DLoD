
#ifndef Camera_h
#define Camera_h

#include <stdio.h>
#include <glm.hpp>

class Camera
{
  public:
  
  Camera();
  
  float azu;
  float alt;
  float radius;
  
  float fov;
  float near;
  float far;
  
  glm::vec3 focalPoint;

  glm::mat4 calculateProjectionMatrix(float asp);
  
  glm::mat4 calculateViewMatrix(glm::vec3);
};







#endif /* Camera_h */
