
#ifndef Camera_h
#define Camera_h

#include "Utility.h"
#include <stdio.h>
#include <assert.h>

class Camera
{
  public:
  
  Camera();
  ~Camera();
  float azu;
  float alt;
  float radius;
  
  float fov;
  float _near;
  float _far;
  
  glm::vec3 focalPoint;

  glm::mat4 calculateProjectionMatrix(float asp);
  
  glm::mat4 calculateViewMatrix(glm::vec3);
};







#endif /* Camera_h */
