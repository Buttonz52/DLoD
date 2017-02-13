#include "Game/main.h"
#include "Physics\PhysXMain.h"


using namespace std;

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer
void RenderGEO(GEO *geo)
{
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(geo->getShader().program);
	glBindVertexArray(geo->getMesh().vertexArray);

	if (geo->hasTexture) {
		geo->getTexture().BindTexture(geo->getShader().program, "sampler");
	}

	vec3 fp = vec3(0, 0, 0);		//focal point

	if (geo->isSkybox || geo->isPlane)
		geo->updateModelMatrix();
	mat4 M = geo->getModelMatrix();
	

	_projection = winRatio * camera->calculateProjectionMatrix((float)width / (float)height);
	_view = camera->calculateViewMatrix();
	
	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(geo->getShader().program, "model"), 1, GL_FALSE, value_ptr(M));
	if (!geo->isSkybox || !geo->isPlane) {
		glUniformMatrix4fv(glGetUniformLocation(geo->getShader().program, "modelview"), 1, GL_FALSE, value_ptr(_view));
	}
	else {
		glUniformMatrix4fv(glGetUniformLocation(geo->getShader().program, "modelview"), 1, GL_FALSE, value_ptr(mat4(mat3(_view))));
	}
	
	glUniformMatrix4fv(glGetUniformLocation(geo->getShader().program, "projection"), 1, GL_FALSE, value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(geo->getShader().program, "lightPosition"), 1, value_ptr(_lightSource));
//	glUniform3fv(glGetUniformLocation(geo->getShader().program, "position"), 1, value_ptr(geo->getPosition()));

	//mesh->texture.BindTexture(shader->program, GL_TEXTURE_2D, "sampler");

	glDrawElements(GL_TRIANGLES, geo->getMesh().elementCount, GL_UNSIGNED_SHORT, 0);
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
	if (geo->hasTexture)
		geo->getTexture().UnbindTexture(GL_TEXTURE_2D);
	// check for an report any OpenGL errors
	CheckGLErrors();
}

//changes between game objects
void changeGEO() {
	geoIndex >= gameObjects.size() - 1 ? geoIndex = 0 : geoIndex++;	//if statement: increment/reset camIndex				is this broken now?
	//currentGEO = gameObjects[geoIndex];

	cout << "Switching control to GEO: " << geoIndex << endl;

}
//changes between cameras
//This is just messing around with stuff, feel free to change implementation/whatnot.
//-Kiersten

void changeCamera() {
	camIndex >= testCams.size()-1 ? camIndex = 0 : camIndex++;	//if statement: increment/reset camIndex 
	camera = &testCams[camIndex];		//new camera

	//testing purposes only
	vec3 *c = camera->getCenter();		//just printing out camera center for testing purposes, delete later
	cout << c->x << " " << c->y << " " << c->z << endl;
}

void clearScreen() {
	// clear screen to a dark grey colour;
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}


// handles keyboard input events when we want multiple keys pressed at once
void AlternKeyCallback(GLFWwindow* window)
{

  int state;

  // Camera movement
  double factor = 0.5;
  state = glfwGetKey(window, GLFW_KEY_I);
  if (state == GLFW_PRESS)
    camera->translate3D(vec3(0, 0, factor));

  state = glfwGetKey(window, GLFW_KEY_K);
  if (state == GLFW_PRESS) 
    camera->translate3D(vec3(0, 0, -factor));

  state = glfwGetKey(window, GLFW_KEY_J);
  if (state == GLFW_PRESS) 
    camera->translate3D(vec3(factor, 0, 0));

  state = glfwGetKey(window, GLFW_KEY_L);
  if (state == GLFW_PRESS) 
    camera->translate3D(vec3(-factor, 0, 0));

  factor = 0.05;

  //Movement of the GEOs
  //state = glfwGetKey(window, GLFW_KEY_UP);
  //if (state == GLFW_PRESS)
  //{
	 // PhysX.accelerate(currentVehicle);
  //}
  //state = glfwGetKey(window, GLFW_KEY_DOWN);
  //if (state == GLFW_PRESS)
  //{
	 // PhysX.decelerate(currentVehicle);
  //}
  //state = glfwGetKey(window, GLFW_KEY_LEFT);
  //if (state == GLFW_PRESS)
  //{
	 // PhysX.turn(currentVehicle, 1);
  //}
  //state = glfwGetKey(window, GLFW_KEY_RIGHT);
  //if (state == GLFW_PRESS)
  //{
	 // PhysX.turn(currentVehicle,-1);
  //}

  //controller version

	 
}


// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float factor = 1.00;
	if (action == GLFW_RELEASE)
		return;

	switch (key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;

	case GLFW_KEY_P:
		audio.PausePlay();
		break;

	case GLFW_KEY_SPACE:
		audio.PlaySfx(audio.horn);
		break;
      
	case GLFW_KEY_G:
		changeGEO();
		break;
    case GLFW_KEY_TAB:
		changeCamera();
      break;

    default:
      break;
	}
}

void GetControllerInput()
{
	testController.Update();

	if (testController.RightTrigger() != 0)
	{
		PhysX.accelerate(currentVehicle, testController.RightTrigger());
	}

	else if (testController.LeftTrigger() != 0)
	{
		PhysX.decelerate(currentVehicle, testController.LeftTrigger());
	}
	else
	{
		PhysX.brake(currentVehicle, 1000.0f);
	}

	PhysX.turn(currentVehicle, testController.LeftStick_X());

	

	// Update for next frame
	//testController.RefreshState();
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
    camera->incrementAzu(dx * 0.005f);
    camera->incrementAlt(dy * 0.005f);
	}
  
	mouse_old_x = x;
	mouse_old_y = y;
}

//handles mouse scroll
void scroll_callback(GLFWwindow* window, double x, double y)
{
	camera->incrementRadius(y * 2);
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

	glfwSwapInterval(1);

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

	//init music
	if (!audio.InitMusic(mainMusic.c_str())) {
		cout << "Failed to load music." << endl;
	}

	if (!audio.PlayMusic()) {
		cout << "Failed to play music" << endl;
	}

	//initialize PhysX
	PhysX.init();

	//initialize 1 game cube, plane, and skybox
	Vehicle* vehicle = initVehicle();
	//GEO* cube = initCube();
	GEO plane = initGroundPlane();
	GEO skybox = initSkyBox();

	camera = &testCams[camIndex];
	currentVehicle = vehicle;
	glEnable(GL_DEPTH_TEST);

	PrintDirections();

	while (!glfwWindowShouldClose(window))
	{
		clearScreen();
		//input

		PhysX.stepPhysics(true, gameObjects);
		
		//update

		camera->followVehicle(vehicle);

		//draw
		RenderGEO(vehicle);
		RenderGEO(&skybox);
		RenderGEO(&plane);
		
		glfwSwapBuffers(window);

		GetControllerInput();
        AlternKeyCallback(window);
		glfwPollEvents();
	}

	vehicle->shutdown();
	PhysX.cleanupPhysics(true);
	audio.CleanUp();

	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

void PrintDirections() {
	cout << "Directions: " << endl;
	cout << "UP/DOWN/LEFT/RIGHT: Move Block" << endl;
	cout << "Xbox R-trigger: acclerate" << endl;
	cout << "Xbox B-Button: decelerate" << endl;
	cout << "Xbox L-Stick: turning" << endl;
	cout << "ESC: Exit program" << endl;
}

Vehicle* initVehicle()
{
	Vehicle* v = new Vehicle();

	v->setFilename("cube.obj");
	if (!v->initMesh()) {
		cout << "Failed to initialize mesh." << endl;
	}
	v->addShaders("shaders/toon.vert", "shaders/toon.frag");

	v->setScale(vec3(10.0f));
	v->setColour(vec3(1, 0, 0));	//red

	if (!v->initBuffers()) {
		cout << "Could not initialize buffers for game object " << v->getFilename() << endl;
	}

	PhysX.initVehicle(v);

	gameObjects.push_back(v);

	return v;
}

GEO initGroundPlane()
{
	GEO plane;
	plane.setFilename("plane.obj");
	if (!plane.initMesh()) {
		cout << "Failed to initialize mesh." << endl;
	}
	plane.setScale(vec3(100.f));
	if (!plane.initTexture("textures/ground.png", GL_TEXTURE_2D)) {
		cout << "Failed to initialize skybox." << endl;
	}
	plane.addShaders("shaders/tex2D.vert", "shaders/tex2D.frag");
	plane.setPosition(vec3(0, -3, 0));

	return plane;
}

GEO initSkyBox()
{
	vector<string> skyboxFiles = {
		"textures/ame_ash/ashcanyon_rt.tga",
		"textures/ame_ash/ashcanyon_lf.tga",
		"textures/ame_ash/ashcanyon_up.tga",
		"textures/ame_ash/ashcanyon_dn.tga",
		"textures/ame_ash/ashcanyon_bk.tga",
		"textures/ame_ash/ashcanyon_ft.tga",
	};

	GEO skybox;
	skybox.setFilename("cube.obj");
	if (!skybox.initMesh()) {
		cout << "Failed to initialize mesh." << endl;
	}
	//scale cube large
	skybox.setScale(vec3(200.f));
	if (!skybox.initSkybox(skyboxFiles)) {
		cout << "Failed to initialize skybox." << endl;
	}
	skybox.addShaders("shaders/skybox.vert", "shaders/skybox.frag");

	return skybox;
}