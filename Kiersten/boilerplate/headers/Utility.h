/* Some OpenGL general methods from boilerplate. */

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

# define PI           3.14159265358979323846  /* pi */
// Specify that we want the OpenGL core profile before including GLFW headers
#ifndef LAB_LINUX
#include <glad/glad.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif
#include <GLFW/glfw3.h>


using namespace std;
using namespace glm;

void QueryGLVersion();
bool CheckGLErrors();

#endif