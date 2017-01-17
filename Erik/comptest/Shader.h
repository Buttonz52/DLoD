

#ifndef Shader_h
#define Shader_h

#include <stdio.h>
#include "utilities.h"
#include <glm.hpp>

class Shader 
{
  // OpenGL names for vertex and fragment shaders, shader program
  GLuint vertex;
  GLuint fragment;
  
  GLint viewLoc;
  GLint projLoc;
  GLint modelLoc;
  GLint widthLoc;
  GLint heightLoc;
  GLint lightLoc;
  
  public:

  GLuint  program;
  void updateView(glm::mat4);
  void updateProj(glm::mat4);
  void updateModel(glm::mat4);
  void updateTextureSize(int,int);
  void togleLighting(bool);
  
  Shader();
  ~Shader();
  
};



#endif /* Shader_hpp */
