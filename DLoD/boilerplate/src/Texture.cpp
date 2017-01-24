#include "GameEngine/Texture.h"

//See STB readme for reason why these two need to be included in the cpp file rather than 
//the header.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
Texture::Texture() {}

Texture::~Texture() {}

bool Texture::InitializeTexture(const char* filename, GLuint target)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &width, &height, &numComponents, 0);
	if (data != nullptr)
	{
		Target = target;
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
		glTexImage2D(Target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Clean up
		glBindTexture(target, 0);
		stbi_image_free(data);
	}
	return !CheckGLErrors();
}
bool Texture::BindTexture(GLuint program, GLuint target, string varName) {
	glActiveTexture(GL_TEXTURE0 + textureID);
	glBindTexture(target, textureID);
	glUniform1i(glGetUniformLocation(program, varName.c_str()), textureID);

	return !CheckGLErrors();
}

void Texture::UnbindTexture(GLuint target) {
	glBindTexture(target, 0);
}
bool Texture::InitializeSkybox(vector<const char*> * filename, GLuint target) {
	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);
	for (int i = 0; i < filename->size(); i++) {
		int numComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filename->at(i), &width, &height, &numComponents, 0);
		if (data != nullptr)
		{
			target = target;

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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
	}

	// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
	// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Clean up
	glBindTexture(target, 0);

	return !CheckGLErrors();
}

// deallocate texture-related objects
void Texture::DestroyTexture()
{
	glBindTexture(Target, 0);
	glDeleteTextures(1, &textureID);
}