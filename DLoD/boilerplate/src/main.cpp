#include "Game/main.h"
#include "Physics\PhysXMain.h"

using namespace std;

//XboxController testController = XboxController(1);

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer
<<<<<<< HEAD
void RenderMesh(GEO *object)
=======
void RenderGEO(GEO *geo)
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043
{
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
<<<<<<< HEAD
	glUseProgram(object->getShader().program);
	glBindVertexArray(object->getMesh().vertexArray);
=======
	glUseProgram(geo->getShader().program);
	glBindVertexArray(geo->getMesh().vertexArray);

	if (geo->hasTexture) {
		geo->getTexture().BindTexture(geo->getShader().program, "sampler");
	}
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043

	vec3 fp = vec3(0, 0, 0);		//focal point

	if (geo->isSkybox || geo->isPlane)
		geo->updateModelMatrix();
	mat4 M = geo->getModelMatrix();
	

	_projection = winRatio * camera->calculateProjectionMatrix((float)width / (float)height);
	_view = camera->calculateViewMatrix();
	
	//uniform variables
<<<<<<< HEAD
	glUniformMatrix4fv(glGetUniformLocation(object->getShader().program, "modelview"), 1, GL_FALSE, value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(object->getShader().program, "projection"), 1, GL_FALSE, value_ptr(_projection));
	glUniform3fv(glGetUniformLocation(object->getShader().program, "lightPosition"), 1, value_ptr(_lightSource));
	glUniform3fv(glGetUniformLocation(object->getShader().program, "position"), 1, value_ptr(object->getPosition()));

	//mesh->texture.BindTexture(shader->program, GL_TEXTURE_2D, "sampler");

	glDrawElements(GL_TRIANGLES, object->getMesh().elementCount, GL_UNSIGNED_SHORT, 0);
=======
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
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043
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
	geoIndex >= gameObjects.size() - 1 ? geoIndex = 0 : geoIndex++;	//if statement: increment/reset camIndex 
	currentGEO = &gameObjects[geoIndex];

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

  //rotations of GEOs
  state = glfwGetKey(window, GLFW_KEY_3);
  if (state == GLFW_PRESS)
    currentGEO->updateRotation(vec3(factor, 0, 0));

  state = glfwGetKey(window, GLFW_KEY_4);
  if (state == GLFW_PRESS)
    currentGEO->updateRotation(vec3(-factor, 0, 0));

  state = glfwGetKey(window, GLFW_KEY_5);
  if (state == GLFW_PRESS)
    currentGEO->updateRotation(vec3(0, factor, 0));

  state = glfwGetKey(window, GLFW_KEY_6);
  if (state == GLFW_PRESS)
    currentGEO->updateRotation(vec3(0, -factor, 0));

  state = glfwGetKey(window, GLFW_KEY_7);
  if (state == GLFW_PRESS)
    currentGEO->updateRotation(vec3(0, 0, factor));

  state = glfwGetKey(window, GLFW_KEY_8);
  if (state == GLFW_PRESS)
    currentGEO->updateRotation(vec3(0, 0, -factor));

  //Movement of the GEOs
  state = glfwGetKey(window, GLFW_KEY_UP);
  if (state == GLFW_PRESS)
  {
	  PhysX.accelerate(currentGEO);
  }
	 
	 
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
<<<<<<< HEAD

	case GLFW_KEY_S:
		audio.PlaySfx(audio.horn);
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
=======
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043

	case GLFW_KEY_SPACE:
		audio.PlaySfx(audio.horn);
		break;
      
	case GLFW_KEY_G:
		changeGEO();
		break;
    case GLFW_KEY_TAB:
		changeCamera();
      break;

	//move GEOs
	case GLFW_KEY_W:
		currentGEO->updatePosition(vec3(0, factor, 0));
		break;

	case GLFW_KEY_A:
		currentGEO->updatePosition(vec3(-factor, 0, 0));
		break;

	case GLFW_KEY_S:
		currentGEO->updatePosition(vec3(0, -factor, 0));
		break;

	case GLFW_KEY_D:
		currentGEO->updatePosition(vec3(factor, 0, 0));
		break;
	case GLFW_KEY_E:
		currentGEO->updatePosition(vec3(0, 0, -factor));
		break;
	case GLFW_KEY_Q:
		currentGEO->updatePosition(vec3(0, 0, factor));
		break;

	//scale
	case GLFW_KEY_1:
		currentGEO->updateScale(vec3(factor*0.01));
		break;
	case GLFW_KEY_2:
		currentGEO->updateScale(vec3(-factor*0.01));
		break;

    default:
      break;
	}
}
/*
void GetControllerInput()
{
	testController.Update();

	if (testController.GetButtonPressed(XBtns.A))
	{
		currentGEO->updatePosition(vec3(0, -0.5f, 0));
	}
	if (testController.GetButtonDown(XBtns.X))
	{
		currentGEO->updatePosition(vec3(-0.5f, 0, 0));
	}
	if (testController.GetButtonDown(XBtns.Y))
	{
		currentGEO->updatePosition(vec3(0, 0.5f, 0));
	}
	if (testController.GetButtonDown(XBtns.B))
	{
		currentGEO->updatePosition(vec3(0.5f, 0, 0));
	}
	// Update for next frame
	testController.RefreshState();
}*/

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

<<<<<<< HEAD

	//// call function to load and compile shader programs

	////meshes[2].texture.InitializeTexture("textures/images/zebra.png", GL_TEXTURE_2D);

	//meshes[3].shader.InitializeShaders("shaders/teapot.vert", "shaders/teapot.frag");
	////glEnable(GL_TEXTURE_2D);
	//if (!meshes[3].Initialize()) {
	//	cout << "ERROR: Could not initialize mesh." << endl;
	//}
	

	//adds a new object for each .obj file in model. populates gameObjects[], only inits the filename
	int numObjFiles = LoadAllObjFiles("models");			
	cout << "Num obj files: " << numObjFiles << endl;

	//initalize all gameObject Meshes, Shaders, textures
	for (int i = 0; i < 4; i++)
	{
		if (!gameObjects[i].getMesh().Initialize())
			cout << "Failed to initialize mesh" << endl;

		gameObjects[3].addMeshShader();
	}
	

	//gameObjects[2] = gameObjects[3];	//another teapot for now

	//if (!audio.InitMusic()) {
	//	cout << "Failed to load music." << endl;
	//}

	//if (!audio.PlayMusic()) {
	//	cout << "Failed to play music" << endl;
	//}

	while (!glfwWindowShouldClose(window))
	{

		//Just renders first mesh for now.
		//meshes[2].texture.BindTexture(meshes[2].program, GL_TEXTURE_2D, "sampler");

		/*RenderMesh(&meshes[3], &meshes[3].shader);*/

		//This code isn't working properly right now.
	/*	_projection = winRatio * camera.calculateProjectionMatrix((float)width / (float)height);
		_view = camera.calculateViewMatrix();
		mesh.cameraInfo._projection = _projection;
		mesh.cameraInfo._view = _view;
		mesh.cameraInfo._lightSource = _lightSource;
		mesh.Render(&shader, &mesh.cameraInfo);*/

		//RenderTriangle(&geometry, &shader);
		//moveCamera()
=======
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
	GEO* cube = initCube();
	GEO plane = initGroundPlane();
	GEO skybox = initSkyBox();

	camera = &testCams[camIndex];
	currentGEO = cube;
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		clearScreen();
		//input
		PhysX.stepPhysics(true);

>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043
		
		//for each (GEO obj in gameObjects)
		//{
		RenderMesh(&gameObjects[3]);
		//}
	

		//update

		//draw
		RenderGEO(cube);
		RenderGEO(&skybox);
		RenderGEO(&plane);
		glfwSwapBuffers(window);

        AlternKeyCallback(window);
		glfwPollEvents();
	}

<<<<<<< HEAD
	// clean up allocated resources before exits
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i].shutdown();
	}
=======
	cube->shutdown();
	PhysX.cleanupPhysics(true);
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043

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

<<<<<<< HEAD
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

				GEO g;
				g.setFilename(entity->d_name);

				gameObjects.push_back(g);
				//gameObjects.emplace_back();
				//gameObjects.at(gameObjects.size()-1).setFilename(entity->d_name);

				numObjFiles++;
			}
		}
	}
	//Unable to find directory.
	else {
		cout << "ERROR LoadAllObjFiles: Directory not found." << endl;
	}
	closedir(dir);
	return gameObjects.size();
}

=======
GEO* initCube()
{
	GEO* cube = new GEO();

	cube->setFilename("cube.obj");
	if (!cube->initMesh()) {
		cout << "Failed to initialize mesh." << endl;
	}
	cube->addShaders("shaders/phong.vert", "shaders/phong.frag");

	cube->setScale(vec3(2.0f));
	cube->setColour(vec3(1, 0, 0));	//red

	if (!cube->initBuffers()) {
		cout << "Could not initialize buffers for game object " << cube->getFilename() << endl;
	}

	//PhysX.initObject(cube);

	return cube;
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
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043
