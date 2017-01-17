
#ifndef Texture_h
#define Texture_h

#include <iostream>
#include <fstream>
#include <string>
#include <GLFW/glfw3.h>
#include <Magick++.h>
#include <OpenGL/gl3.h>


using namespace std;

struct Texture
{
  // OpenGL names for array buffer objects, vertex array object
  GLuint  textureName;
  GLenum  textureTarget;
  // dimensions of the image stored in this texture
  GLuint  width, height;
  
  // Constructor to replace initialize Texture
  Texture(const string &imageFileName, GLenum target = GL_TEXTURE_RECTANGLE);
    // Deconstruction for the MyGeometry struct
  ~Texture();
};


#endif /* Texture_h */
