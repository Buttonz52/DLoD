
#include <stdio.h>
#include "Texture.h"
#include "Utilities.h"


Texture::Texture(const string &imageFileName, GLenum target)
{
  Magick::Image myImage;
  
  // try to read the provided image file
  try {
    myImage.read(imageFileName);
  }
  catch (Magick::Error &error) {
    cout << "Magick++ failed to read image " << imageFileName << endl;
    cout << "ERROR: " << error.what() << endl;
    abort();
  }
  
  // store the image width and height into the texture structure
  width = (GLuint) myImage.columns();
  height = (GLuint) myImage.rows();

  textureTarget = target;

  // create a Magick++ pixel cache from the image for direct access to data
  Magick::Pixels pixelCache(myImage);
  Magick::PixelPacket *pixels = pixelCache.get(0, 0, width, height);

  // determine the number of stored bytes per pixel channel in the cache
  GLenum channelDataType;
  switch (sizeof(Magick::Quantum)) {
    case 4:     channelDataType = GL_UNSIGNED_INT;      break;
    case 2:     channelDataType = GL_UNSIGNED_SHORT;    break;
    default:    channelDataType = GL_UNSIGNED_BYTE;
  }

  // create a texture name to associate our image data with
  glGenTextures(1, &textureName);
  // bind the textureGL_TEXTURE_RECTANGLEas a "rectangle" to access using image pixel coordinates
  glBindTexture(textureTarget, textureName);

  // send image pixel data to OpenGL texture memory
  glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_BGRA, channelDataType, pixels);

  // unbind this texture
  glBindTexture(textureTarget, 0);
  
  CheckGLErrors();

}

Texture::~Texture() 
{
  if (textureName != 0) {
    glDeleteTextures(1, &textureName);
  }
}

