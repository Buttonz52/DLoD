
#include "SphereNode.h"



SphereNode::SphereNode(Texture* tex, bool b, float r, float orbitR, float orbitP, float rotP, float axiTil) : Entity(tex, b)
{
  radius = r;
  orbitalRadius = orbitR;
  orbitalPeriod = orbitP;
  rotationalPeriod = rotP;
  axialTilt = axiTil;
}


static void pushTriangle(glm::vec3 a, glm::vec2 texa, glm::vec3 b, glm::vec2 texb, glm::vec3 c, glm::vec2 texc, vector<GLfloat>* points) {
  
  // push the position and texture cordinates for point a
  pushVec3(points, a);
  pushVec2(points, texa);

  // push the position and texture cordinates for point b
  pushVec3(points, b);
  pushVec2(points, texb);
  
  // push the poistion and texture cordinates for point c
  pushVec3(points, c);
  pushVec2(points, texc);
}


vector<GLfloat>* SphereNode::calculatePoints() {
  vector<GLfloat>* points = new vector<GLfloat>();
  int k = 24;
  
  // Draw top of sphere
  glm::vec3 top = glm::vec3(0,1,0);
  
  for (int i = 0; i < 2 * k; ++i) {
    float azu1 = i * M_PI / k;
    float azu2 = (i + 1) * M_PI / k;
    float alt = M_PI / k;
    
    float x1 = sin(alt) * cos(azu1);
    float y1 = cos(alt);
    float z1 = sin(alt) * sin(azu1);
    
    float x2 = sin(alt) * cos(azu2);
    float y2 = cos(alt);
    float z2 = sin(alt) * sin(azu2);

    float texx1 = (float) i / (float) (2 * k);
    float texx2 = (float) (i + 1) / (float) (2 * k);
    
    float texy1 = 0;
    float texy2 = (float) 1 / (float) k;
    
    glm::vec2 texTop = glm::vec2(texx1, texy1);
    glm::vec2 tex1 = glm::vec2(texx1, texy2);
    glm::vec2 tex2 = glm::vec2(texx2, texy2);
    
    glm::vec3 t1 = glm::vec3(x1,y1,z1);
    glm::vec3 t2 = glm::vec3(x2,y2,z2);
    
    pushTriangle(top, texTop, t2, tex2, t1, tex1, points);
  }
  
  // Draw the middle of the sphere
  
  for (int i = 1; i < k-1; ++i) {
    for (int j = 0; j < 2*k; ++j) {
      float azu1 = j * M_PI / k;
      float azu2 = (j + 1) * M_PI / k;
      float alt1 = i * M_PI / k;
      float alt2 = (i+1) * M_PI / k;
      
      float x1 = sin(alt1) * cos(azu1);
      float y1 = cos(alt1);
      float z1 = sin(alt1) * sin(azu1);
      
      float x2 = sin(alt1) * cos(azu2);
      float y2 = cos(alt1);
      float z2 = sin(alt1) * sin(azu2);
      
      float x3 = sin(alt2) * cos(azu1);
      float y3 = cos(alt2);
      float z3 = sin(alt2) * sin(azu1);
      
      float x4 = sin(alt2) * cos(azu2);
      float y4 = cos(alt2);
      float z4 = sin(alt2) * sin(azu2);
      
      float texx1 = (float) j / (float) (2 * k);
      float texx2 = (float) (j + 1) / (float) (2 * k);

      float texy1 = (float) i / (float) k;
      float texy2 = (float) (i + 1) / (float) k;
     
      glm::vec2 tex1 = glm::vec2(texx1, texy1);
      glm::vec2 tex2 = glm::vec2(texx2, texy1);
      glm::vec2 tex3 = glm::vec2(texx1, texy2);
      glm::vec2 tex4 = glm::vec2(texx2, texy2);
      
      glm::vec3 t1 = glm::vec3(x1,y1,z1);
      glm::vec3 t2 = glm::vec3(x2,y2,z2);
      glm::vec3 t3 = glm::vec3(x3,y3,z3);
      glm::vec3 t4 = glm::vec3(x4,y4,z4);
      
      pushTriangle(t1, tex1, t2, tex2, t3, tex3, points);
      pushTriangle(t2, tex2, t4, tex4, t3, tex3, points);
    }
  }
  
  // Draw bottom of sphere
  glm:: vec3 bottom = - glm::vec3(0,1,0);
  
  for (int i = 0; i < 2 * k; ++i) {
    float azu1 = i * M_PI / k;
    float azu2 = (i + 1) * M_PI / k;
    float alt = M_PI - M_PI / k;
    
    float x1 = sin(alt) * cos(azu1);
    float y1 = cos(alt);
    float z1 = sin(alt) * sin(azu1);
    
    float x2 = sin(alt) * cos(azu2);
    float y2 = cos(alt);
    float z2 = sin(alt) * sin(azu2);

    float texx1 = (float) i / (float) (2 * k);
    float texx2 = (float) (i + 1) / (float) (2 * k);
    
    float texy1 = (float) (k - 1) / (float) k;
    float texy2 = 1;
    
    glm::vec2 tex1 = glm::vec2(texx1, texy1);
    glm::vec2 tex2 = glm::vec2(texx2, texy1);
    glm::vec2 texBot = glm::vec2(texx2, texy2);

    glm::vec3 t1 = glm::vec3(x1,y1,z1);
    glm::vec3 t2 = glm::vec3(x2,y2,z2);

    pushTriangle(bottom, texBot, t1, tex1, t2, tex2, points);
  }
  return points;
}

GLenum SphereNode::primitiveType() {
  return GL_TRIANGLES;
}

/* Updates the model and translations matricies given the current time
 *
 */
void SphereNode::update(float time) {
  glm::mat4 xrotation = glm::mat4(1);
  glm::mat4 yrotation = glm::mat4(1);
  glm::mat4 zrotation = glm::mat4(1);
  glm::mat4 translation = glm::mat4(1);
  glm::mat4 scale = glm::mat4(1);
  
  if (rotationalPeriod != 0) {
    yrotation[0][0] = cos(2 * M_PI * time / rotationalPeriod);
    yrotation[0][2] = -sin(2 * M_PI * time / rotationalPeriod);
    yrotation[2][0] = sin(2 * M_PI * time / rotationalPeriod);
    yrotation[2][2] = cos(2 * M_PI * time / rotationalPeriod);
  }

  if (axialTilt != 0) {
    xrotation[1][1] = cos(axialTilt * M_PI / 180);
    xrotation[1][2] = -sin(axialTilt * M_PI / 180);
    xrotation[2][1] = sin(axialTilt * M_PI / 180);
    xrotation[2][2] = cos(axialTilt * M_PI / 180);
  }
  
  if (orbitalPeriod != 0) {
    glm::vec3 pos = position(time);
    
    translation[3][0] = pos.x;
    translation[3][1] = pos.y;
    translation[3][2] = pos.z;
  }
  
  scale[0][0] = radius;
  scale[1][1] = radius;
  scale[2][2] = radius;
  
  modelMatrix = translation * scale * xrotation * yrotation * zrotation;
  translationMatrix = translation * xrotation;
  
  for (int i = 0; i < children.size(); ++i) {
    children[i]->update(time);
  }
  
}

glm::vec3 SphereNode::position(float time) {
  float x = orbitalRadius * cos(2 * M_PI * time / orbitalPeriod);
  float z = orbitalRadius * sin(2 * M_PI * time / orbitalPeriod);
  
  return glm::vec3(x,0,z);
}

























