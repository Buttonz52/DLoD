
#ifndef SphereNode_h
#define SphereNode_h

#include <stdio.h>
#include "Entity.h"
#include "Utilities.h"




class SphereNode : public Entity {
  
  float radius;
  float orbitalRadius;
  float orbitalPeriod;
  float rotationalPeriod;
  float axialTilt;
  
  static vector<float> sphericalPoints;  
public:
  SphereNode(Texture* tex, bool b, float r, float orbitR, float orbitP, float rotP, float axiTil);
  void update(float time);
  glm::vec3 position(float time);

protected:
  virtual vector<GLfloat>* calculatePoints();
  virtual GLenum primitiveType();
  
};






#endif /* SphereNode_h */
