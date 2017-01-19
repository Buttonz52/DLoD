
#ifndef Camera_h
#define Camera_h

#include <stdio.h>
#include <glm.hpp>

#include "SphereNode.h"

class Camera
{
private:
  
  float azu;
  float alt;
  float radius;
  
  float fov;
  float near;
  float far;
 
public:
  
  Camera();

  
  void setAzu(float);
  void setAlt(float);
  void setRadius(float);
  
  void incrementAzu(float);
  void incrementAlt(float);
  void incrementRadius(float);
  
  

  glm::vec3 focalPoint;

  glm::mat4 calculateProjectionMatrix(float asp);
  
  glm::mat4 calculateViewMatrix();
  
  void moveCameraBy(glm::vec3);
};







#endif /* Camera_h */
