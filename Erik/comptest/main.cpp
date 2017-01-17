/*

  https://github.com/Buttonz52/DLoD.git

*/

#include <stdio.h>

#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <iterator>
#include <assert.h>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
//#include <OpenGL/gl3.h>
//#include <Magick++.h>
#include "Entity.h"
#include "Utilities.h"
#include "Camera.h"
#include "SphereNode.h"
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define M_PI 3.1415
#define M_PI_4 3.1415/4
using namespace std;

Camera* camera;
bool wireframe;
SphereNode* center;

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
  cout << "GLFW ERROR " << error << ":" << endl;
  cout << description << endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_RELEASE)
    return;

  switch (key) {
    case GLFW_KEY_ESCAPE :
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
      
    case GLFW_KEY_RIGHT:
      camera->azu += M_PI / 180;
      break;

    case GLFW_KEY_LEFT:
      camera->azu -= M_PI / 180;
      break;

    case GLFW_KEY_UP:
      camera->alt = min(max((float) (camera->alt - M_PI / 180), 0.01f), (float) M_PI - 0.01f);
      break;
      
    case GLFW_KEY_DOWN:
      camera->alt = min(max((float) (camera->alt + M_PI / 180), 0.01f), (float) M_PI - 0.01f);
      break;

    case GLFW_KEY_W:
      wireframe = !wireframe;
      break;

      
    default:
      break;
  }
}

void scrollCallback(GLFWwindow* window, double x, double y) {
  camera->radius = min(max((float) (camera->radius - y), 1.0f),70.0f);
}



int main(int argc, char *argv[])
{
  // initialize the GLFW windowing system
  if (!glfwInit()) {
    cout << "ERROR: GLFW failed to initilize, TERMINATING" << endl;
    return -1;
  }
  glfwSetErrorCallback(ErrorCallback);

  // attempt to create a window with an OpenGL 4.1 core profile context
  GLFWwindow *window = 0;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(1024, 1024, "Derby League of Destruction", 0, 0);
  if (!window) {
    cout << "Program failed to create GLFW window, TERMINATING" << endl;
    glfwTerminate();
    return -1;
  }

  // set keyboard callback function and make our context current (active)
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwMakeContextCurrent(window);

  if (!gladLoadGL())
  {
	  cout << "GLAD init failed" << endl;
	  return -1;
  }

  // query and print out information about our OpenGL environment
  QueryGLVersion();


  camera = new Camera();
  camera->azu = M_PI_4;
  
  Shader* shader = new Shader();

  Texture* sunTex = new Texture("sun.jpg");
  Entity* scene = new SphereNode(sunTex, false, 5, 0,0,0,0);
  
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  CheckGLErrors();
  
  // run an event-triggered main loop
  while (!glfwWindowShouldClose(window))
  {
    // clear screen to a dark grey colour
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClearDepth(std::numeric_limits<float>::is_signed);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    wireframe ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);

    int width;
    int height;
    
    glfwGetWindowSize(window, &width, &height);
    
    glm::vec3 fp = glm::vec3(0,0,0);
    
    // scene->render()
    shader->updateProj(camera->calculateProjectionMatrix((float)width / (float)height));
    shader->updateView(camera->calculateViewMatrix(fp));
    scene->update(0);
    scene->render(glm::mat4(1),shader);

    glfwSwapBuffers(window);
    
    // sleep until next event before drawing again
    glfwPollEvents();
  }

  // clean up allocated resources before exit
  glfwDestroyWindow(window);
  glfwTerminate();

  cout << "Goodbye!" << endl;
  return 0;
}
