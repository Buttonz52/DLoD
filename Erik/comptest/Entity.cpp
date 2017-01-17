
#include "Entity.h"


Entity::Entity(Texture* tex, bool b)
{
  assert(tex != 0);

  modelMatrix = glm::mat4(1);
  texture = tex;
  initialized = false;
  enableLight = b;
  winding = GL_CCW;
}


Entity::~Entity()
{
  if (vertexArray != 0) {
    glDeleteVertexArrays(1, &vertexArray);
  }
  if (vertexBuffer != 0) {
    glDeleteBuffers(1, &vertexBuffer);
  }
}


void Entity::initialize()
{
  vector<GLfloat>* points = this->calculatePoints();
  vertexCount = (GLsizei)points->size() / 5;
  
  // create an array buffer object for storing our vertices
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  GLsizei bufferSize = (GLsizei)points->size() * sizeof(GLfloat);
  GLfloat *bufferPtr = points->data();
  glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferPtr, GL_STATIC_DRAW);
  
  // create a vertex array object encapsulating all our vertex attributes
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);
  
  // associate the position array with the vertex array object
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(VERTEX_INDEX);
  glVertexAttribPointer(TEXTURE_INDEX, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(TEXTURE_INDEX);
  
  // unbind our buffers, resetting to default state
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  CheckGLErrors();
  
  delete points;
  
  initialized = true;
}


/* Draw the Entity
 *
 */
void Entity::render(glm::mat4 parentModelMatrix, Shader* shader)
{
  if (!initialized)
    initialize();
  
  shader->updateModel(parentModelMatrix * modelMatrix);
  shader->updateTextureSize(texture->width, texture->height);
  shader->togleLighting(enableLight);
  
  glUseProgram(shader->program);
  glBindVertexArray(vertexArray);
  glBindTexture(texture->textureTarget, texture->textureName);
  glFrontFace(winding);
  glDrawArrays(this->primitiveType(), 0, vertexCount);
  glBindTexture(texture->textureTarget, 0);
  glBindVertexArray(0);

  glUseProgram(0);
  
  for (int i = 0; i < children.size(); ++i) {
    children[i]->render(parentModelMatrix * translationMatrix, shader);
  }

}


void Entity::update(float time)
{
  
}
























