#include "headers/Texture.h"

Texture::Texture()
{
	textureID = 0;
	target = 0;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	DestroyTexture();
}

// Functions to set up OpenGL buffers for storing textures
bool Texture::InitializeTexture(const char* filename, GLuint target)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &width, &height, &numComponents, 0);
	if (data != nullptr)
	{
		target = target;
		glGenTextures(1, &textureID);
		glBindTexture(target, textureID);
		GLuint format = GL_RGB;
		switch (numComponents)
		{
		case 4:
			format = GL_RGBA;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 2:
			format = GL_RG;
			break;
		case 1:
			format = GL_RED;
			break;
		default:
			cout << "Invalid Texture Format" << endl;
			break;
		};
		glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Clean up
		glBindTexture(target, 0);
		stbi_image_free(data);
		return !CheckGLErrors();
	}
	return true; //error
}

// deallocate texture-related objects
void Texture::DestroyTexture()
{
	glBindTexture(target, 0);
	glDeleteTextures(1, &textureID);
}
