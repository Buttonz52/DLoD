
#ifndef GeometryNode_h
#define GeometryNode_h

#include <stdio.h>
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include <vector>
#include "Utilities.h"


class Entity 
{
  static const GLuint VERTEX_INDEX = 0;
  static const GLuint TEXTURE_INDEX = 1;

  // OpenGL names for array buffer objects, vertex array object
  GLuint  vertexBuffer;
  GLuint  vertexArray;
  bool initialized;
  bool enableLight;

  GLsizei vertexCount;
  
  void initialize();

public:
  GLenum winding;
  glm::mat4 modelMatrix;
  glm::mat4 translationMatrix;
  Texture* texture;
 
  vector<Entity*> children;
  
  Entity(Texture* tex, bool b);
  ~Entity();
  
  void render(glm::mat4, Shader*);

  virtual void update(float time);
  virtual glm::vec3 position(float time) = 0;
  
protected:
  virtual vector<GLfloat>* calculatePoints() = 0;
  virtual GLenum primitiveType() = 0;
  
};



#endif /* GeometryNode_h */
