#include "Game/main.h"

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer
void RenderMesh(Mesh *mesh, Shader *shader)
{
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader->program);
	glBindVertexArray(mesh->vertexArray);

	vec3 fp = vec3(0, 0, 0);		//focal point

	_projection = winRatio * camera.calculateProjectionMatrix((float)width / (float)height);
	_view = camera.calculateViewMatrix();
	
	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "modelview"), 1, GL_FALSE, value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(shader->program, "lightPosition"), 1, value_ptr(_lightSource));

	glDrawElements(GL_TRIANGLES, mesh->elementCount, GL_UNSIGNED_SHORT, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	switch (key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
      
    case GLFW_KEY_I:
      camera.translate3D(vec3(0,0.2,0));
      break;
      
    case GLFW_KEY_K:
      camera.translate3D(vec3(0,-0.2,0));
      break;
      
    case GLFW_KEY_J:
      camera.translate3D(vec3(-0.2,0,0));
      break;
      
    case GLFW_KEY_L:
      camera.translate3D(vec3(0.2,0,0));
      break;

    case GLFW_KEY_RIGHT:
      camera.incrementAzu(-M_PI/180);
      break;
      
    case GLFW_KEY_LEFT:
      camera.incrementAzu(M_PI/180);
      break;
      
    case GLFW_KEY_UP:
      camera.incrementAlt(-M_PI/180);
      break;
      
    case GLFW_KEY_DOWN:
      camera.incrementAlt(M_PI/180);
      break;

    case GLFW_KEY_A:
	  _rotate_y += 1.0f;
      break;
    case GLFW_KEY_D:
	  _rotate_y -= 1.0f;
      break;

    default:
      break;
	}
}

// handles mouse click
void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouse_old_x = x;
		mouse_old_y = y;
	}
}

// handles delta mouse click move
void motion(GLFWwindow* w, double x, double y)
{

	double dx, dy;
	dx = (x - mouse_old_x);
	dy = (y - mouse_old_y);

	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_1))
	{
    camera.incrementAzu(dx * 0.005f);
    camera.incrementAlt(dy * 0.005f);
	}
  
	mouse_old_x = x;
	mouse_old_y = y;
}

//handles mouse scroll
void scroll_callback(GLFWwindow* window, double x, double y)
{
	camera.incrementRadius(y / 2);
}

//Jeremy Hart, CPSC 587 code, handles resizing glfw window
void resizeCallback(GLFWwindow* window, int width, int height)
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	glViewport(0, 0, width, height);

	float minDim = float(min(width, height));

	winRatio[0][0] = minDim / float(width);
}

// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Derby League of Destruction", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetCursorPosCallback(window, motion);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowSizeCallback(window, resizeCallback);

	//Intialize GLAD
	#ifndef LAB_LINUX
	if (!gladLoadGL())
	{
		cout << "GLAD init failed" << endl;
		return -1;
	}
	#endif

	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	int numObjFiles = LoadAllObjFiles("models");
	cout << "Num obj files: " << numObjFiles << endl;

	//Just loads for meshes[0] for now.
	if (!meshes[2].shader.InitializeShaders("shaders/mesh.vert", "shaders/mesh.frag")) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
	}
 
	if (!meshes[2].Initialize()) {
		cout << "ERROR: Could not initialize mesh." << endl;
	}
	while (!glfwWindowShouldClose(window))
	{
		// call function to draw our scene
		/*
		if(g_play){
		t += dt;
		animateQuad(t);
		}
		*/
		
		//Just renders first mesh for now.
		RenderMesh(&meshes[2], &meshes[2].shader);

		//This code isn't working properly right now.
	/*	_projection = winRatio * camera.calculateProjectionMatrix((float)width / (float)height);
		_view = camera.calculateViewMatrix();
		mesh.cameraInfo._projection = _projection;
		mesh.cameraInfo._view = _view;
		mesh.cameraInfo._lightSource = _lightSource;
		mesh.Render(&shader, &mesh.cameraInfo);*/

		//RenderTriangle(&geometry, &shader);
		//moveCamera()
		

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up allocated resources before exits
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].DestroyMesh();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

void PrintDirections() {
	cout << "Directions: " << endl;
	cout << "A: Rotate object ccw \nD: Rotate object cw" << endl;
	cout << "UP/DOWN/LEFT/RIGHT: Rotate camera" << endl;
	cout << "ESC: Exit program" << endl;
}

//Loads meshes
int LoadAllObjFiles(const char *pathname) {

	//Get the files in the directory desired.
	//Resource: http://www.cplusplus.com/forum/beginner/9173/
	DIR *dir;
	struct dirent *entity;
	int numObjFiles =0;
	//Check if directory exists
	if ((dir = opendir(pathname)) != NULL) {
		size_t found;
		//Check if entities exist.
		while ((entity = readdir(dir)) != NULL) {
			//Only parse files with .obj
			//ASSUMPTION: People are smart enough to only have
			//.obj at end of file.

			//TODO: add extensions for different types of 
			//mesh files.
			found = string(entity->d_name).find(".obj");
			if (found!=string::npos) 
			{
				//Get path name for parsing file
				//Set to 30 arbitrarily

				//Resource: http://www.cplusplus.com/reference/cstring/strcat/
				char s[30];
				strcpy(s, pathname);
				strcat(s, "/");
				strcat(s, entity->d_name);
				//Add mesh to mesh vector
				AddMesh(&string(s));
				numObjFiles++;
			}
		}
	}
	//Unable to find directory.
	else {
		cout << "ERROR LoadAllObjFiles: Directory not found." << endl;
	}
	closedir(dir);
	return numObjFiles;
}

//Adds mesh file to mesh vector based on directory
void AddMesh(const string *pathname) {
	Mesh mesh;
	//Get all information for mesh
	mesh.ReadMesh(*pathname);

	//Add colour for the moment; this can be taken out
	//or colour changed/colour added to obj files and 
	//not here
	vec3 red(1.f, 0.f, 0.f);
	mesh.AddColour(&red);

//	if (!mesh.shader.InitializeShaders("shaders/mesh.vert", "shaders/mesh.frag")) {
	//	cout << "Program could not initialize shaders, TERMINATING" << endl;
	//}

	meshes.push_back(mesh);
	cout << "Loaded " << *pathname << endl;
}
