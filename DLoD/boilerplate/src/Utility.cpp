#include "Game/Utility.h"

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}


//Normalizes a vec3
vec3 Normalize(const vec3 &v) {
	float magn = v.x*v.x + v.y*v.y + v.z*v.z;
	if (magn <= 0) {
		return vec3(0);
	}

	return (v / sqrt(magn));
}

bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		//yells at you only if you are in debug mode
#if DEBUG
	//	cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM" << endl; 
			break;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE" << endl; 
			break;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION" << endl; 
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
#endif
		error = true;
	}
	return error;
}

void print4x4Matrix(mat4 mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << mat[i][j] << " ";
		}
		cout << "" << endl;
	}
	printf("\n");
}