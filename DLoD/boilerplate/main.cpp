// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================

#include "headers/main.h"

// --------------------------------------------------------------------------

// Functions to set up OpenGL buffers for storing geometry data

struct MyGeometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint normalBuffer;
	GLuint indicesBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;

	// initialize object names to zero (OpenGL reserved value)
	MyGeometry() : vertexBuffer(0), textureBuffer(0), normalBuffer(0), colourBuffer(0), indicesBuffer(0), vertexArray(0), elementCount(0)
	{}
};

// create buffers and fill with geometry data, returning true if successful
bool InitializeGeometry(MyGeometry *geometry)
{
	//_vertices.clear();
	//_colours.clear();
	//_normals.clear();
	//_faces.clear();
	//_uv.clear();
	Mesh mesh;
	mesh.ReadMesh("models/teapot.obj");
//void LoadObject(string &filename, vector<vec3> &vertices, vector<vec3> &normals, vector<vec3> &faces,  vector<vec3> &uv) {
	//ObjLoader obj;
	//obj loader works for teapot, not for sphere.
	//obj.LoadObject("models/teapot.obj", _vertices, _normals, _faces, _uv);

	//Adding red colour for all vertices so it shows up. 
	vec3 red(1.f, 0.f, 0.f);
	mesh.AddColour(&red);
	// three vertex positions and assocated colours of a triangle
	/*vec3 v1(-0.6f, -0.4f, -1.f);
	vec3 v2(0.f, 0.6f, 0.f);
	vec3 v3(0.6f, -0.4f, 1.f);

	_vertices.push_back(v1);
	_vertices.push_back(v2);
	_vertices.push_back(v3);


	vec3 c1(1.0f, 0.0f, 0.0f);
	vec3 c2(0.0f, 1.0f, 0.0f);
	vec3 c3(0.0f, 0.0f, 1.0f);
	
	_colours.push_back(c1);
	_colours.push_back(c2);
	_colours.push_back(c3);*/

		//geometry->elementCount = _faces.size() * 3;
		geometry->elementCount = mesh.faces.size();
	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;
	const GLuint NORMAL_INDEX = 2;

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(vec3), mesh.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_INDEX);

	// create another one for storing our colours
	glGenBuffers(1, &geometry->colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.colours.size() * sizeof(vec3), mesh.colours.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(COLOUR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(COLOUR_INDEX);

	// create another one for storing our colours
	glGenBuffers(1, &geometry->normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(vec3), mesh.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(NORMAL_INDEX);

	//Indices buffer.
	glGenBuffers(1, &geometry->indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(GLushort), mesh.faces.data(), GL_STATIC_DRAW);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

// deallocate geometry-related objects
void DestroyGeometry(MyGeometry *geometry)
{
	// unbind and destroy our vertex array object and associated buffers
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &geometry->vertexArray);
	glDeleteBuffers(1, &geometry->vertexBuffer);
	glDeleteBuffers(1, &geometry->colourBuffer);
	glDeleteBuffers(1, &geometry->indicesBuffer);
	glDeleteBuffers(1, &geometry->normalBuffer);
	glDeleteBuffers(1, &geometry->textureBuffer);
}

// --------------------------------------------------------------------------
// Function that rotates the object around the y axis.
void RotateObject(float factor) {
	_rotate_y += factor;

}
// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

void RenderMesh(Mesh *mesh, MyShader *shader)
{
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader->program);
	glBindVertexArray(mesh->vertexArray);

	vec3 fp = vec3(0, 0, 0);

	_projection = winRatio * camera.calculateProjectionMatrix((float)width / (float)height);
	_view = camera.calculateViewMatrix();
	
	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "modelview"), 1, GL_FALSE, glm::value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(shader->program, "lightPosition"), 1, glm::value_ptr(_lightSource));

	//Drawing obj files doesn't work yet.
	//glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

	//JAN 16: This works now. //This is in Allan's code, but nothing appears.  TODO: Fix it.
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
      RotateObject(1.0f);
      break;
    case GLFW_KEY_D:
      RotateObject(-1.0f);
      break;

    default:
      break;
	}
}

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

void scroll_callback(GLFWwindow* window, double x, double y)
{
	camera.incrementRadius(y / 2);
}

//Jeremy Hart, CPSC 587 code
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

	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
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
	Camera camera;
	// call function to load and compile shader programs
	MyShader shader;
	if (!InitializeShaders(&shader)) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}

	// call function to create and fill buffers with geometry data
	MyGeometry geometry;
	//if (!InitializeGeometry(&geometry))
	//	cout << "Program failed to intialize geometry!" << endl;

	// run an event-triggered main loop
	PrintDirections();

	int numObjFiles;

	LoadAllObjFiles("models");

	//Read mesh contents
	//mesh.ReadMesh("models/teapot.obj");

	//void LoadObject(string &filename, vector<vec3> &vertices, vector<vec3> &normals, vector<vec3> &faces,  vector<vec3> &uv) {
	//ObjLoader obj;
	//obj loader works for teapot, not for sphere.
	//obj.LoadObject("models/teapot.obj", _vertices, _normals, _faces, _uv);

	//Adding red colour for all mesh's vertices so it shows up. 
	if (!meshes[0].Initialize()) {
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
		RenderMesh(&meshes[0], &shader);

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

	// clean up allocated resources before exit
	DestroyGeometry(&geometry);
	DestroyShaders(&shader);
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
void LoadAllObjFiles(const char *pathname) {

	//Get the files in the directory desired.
	//Resource: http://www.cplusplus.com/forum/beginner/9173/
	DIR *dir;
	struct dirent *entity;
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
			}
		}
	}
	//Unable to find directory.
	else {
		cout << "ERROR LoadAllObjFiles: Directory not found." << endl;
	}
	closedir(dir);

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

	meshes.push_back(mesh);
	cout << "Loaded " << *pathname << endl;
}