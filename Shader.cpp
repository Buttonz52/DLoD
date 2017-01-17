
#include "Shader.h"

Shader::Shader() {
  // load shader source from files
  string vertexSource = LoadSource("vertex.glsl");
  string fragmentSource = LoadSource("fragment.glsl");
  if (vertexSource.empty() || fragmentSource.empty()) 
    abort();

  // compile shader source into shader objects
  vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
  fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

  // link shader program
  program = LinkProgram(vertex, fragment);
  
  viewLoc = glGetUniformLocation(program, "view");
  projLoc = glGetUniformLocation(program, "proj");
  modelLoc = glGetUniformLocation(program, "model");
  widthLoc = glGetUniformLocation(program, "width");
  heightLoc = glGetUniformLocation(program, "height");
  lightLoc = glGetUniformLocation(program, "lightEnable");
}
  
  
Shader::~Shader()
{
  if (program != 0) {
    glDeleteProgram(program);
  }
  if (vertex != 0) {
    glDeleteShader(vertex);
  }
  if (fragment != 0) {
    glDeleteShader(fragment);
  }
}

void Shader::updateView(glm::mat4 matrix){
  glUseProgram(program);
  glUniformMatrix4fv(viewLoc, 1, false, &matrix[0][0]);
  glUseProgram(0);
}

void Shader::updateProj(glm::mat4 matrix){
  glUseProgram(program);
  glUniformMatrix4fv(projLoc, 1, false, &matrix[0][0]);
  glUseProgram(0);
}

void Shader::updateModel(glm::mat4 matrix){
  glUseProgram(program);
  glUniformMatrix4fv(modelLoc, 1, false, &matrix[0][0]);
  glUseProgram(0);
}

void Shader::updateTextureSize(int width, int height) {
  glUseProgram(program);
  glUniform1i(widthLoc, width);
  glUniform1i(heightLoc, height);
  glUseProgram(0);
}

void Shader::togleLighting(bool b) {
  glUseProgram(program);
  if (b) {
    glUniform1i(lightLoc, 1);
  }
  else {
    glUniform1i(lightLoc, -1);
  }
  glUseProgram(0);
}
