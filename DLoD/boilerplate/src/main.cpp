#include "Game/main.h"

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

	mat4 rotation = geo->getRotation();	

	vec3 fp = vec3(0, 0, 0);		//focal point
	mat4 scale = glm::scale(geo->getScale());
	//mat4 rotation = glm::rotate(mat4(), 0.f, vec3(1));	//no rotation for now; TODO: Implement properly later
	mat4 translation = translate(geo->getPosition());

	mat4 M = translation * rotation * scale;
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


  //Movement of the geos -- maybe
}


// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float factor = 0.05;
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
      
    case GLFW_KEY_RIGHT:
      camera->incrementAzu(-M_PI/180);
      break;
      
    case GLFW_KEY_LEFT:
      camera->incrementAzu(M_PI/180);
      break;
      
    case GLFW_KEY_UP:
      camera->incrementAlt(-M_PI/180);
      break;
      
    case GLFW_KEY_DOWN:
      camera->incrementAlt(M_PI/180);
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


	//// call function to load and compile shader programs

	////meshes[2].texture.InitializeTexture("textures/images/zebra.png", GL_TEXTURE_2D);

	//meshes[3].shader.InitializeShaders("shaders/teapot.vert", "shaders/teapot.frag");
	////glEnable(GL_TEXTURE_2D);
	//if (!meshes[3].Initialize()) {
	//	cout << "ERROR: Could not initialize mesh." << endl;
	//}


	//adds a new object for each .obj file in model. populates gameObjects[], only inits the filename
	//int numObjFiles = LoadAllObjFiles("models");
	//cout << "Num obj files: " << numObjFiles << endl;


	//initialize 5 game objects and push them onto vector for now
	GEO g;
	gameObjects.push_back(g);
	gameObjects.push_back(g);
	gameObjects.push_back(g);
	gameObjects.push_back(g);
	gameObjects.push_back(g);

	//initialize game object meshes
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i].setFilename("teapot.obj");
		if (!gameObjects[i].initMesh()) {
			cout << "Failed to initialize mesh." << endl;
		}
		gameObjects[i].addShaders("shaders/teapot.vert", "shaders/teapot.frag");
		gameObjects[i].setScale(vec3(0.05f));
	}

	//set colours
	gameObjects[0].setColour(vec3(0,1,0));	//green
	gameObjects[1].setColour(vec3(1,0,0));	//red
	gameObjects[2].setColour(vec3(0, 0, 1));	//blue
	gameObjects[3].setColour(vec3(1, 1, 0));	//yellow
	gameObjects[4].setColour(vec3(1, 0, 1));	//magenta

	//set positions
	gameObjects[1].setPosition(vec3(-15,0,-50));	//this doesn't set the position
	gameObjects[2].setPosition(vec3(0, 20, 0));	//this doesn't set the position
	gameObjects[3].setPosition(vec3(-15, 0, 0));	//this doesn't set the position
	gameObjects[4].setPosition(vec3(0, -20, 10));	//this doesn't set the position

	//texture game object 2
	if (!gameObjects[2].initTexture("textures/brick_wall_png.png", GL_TEXTURE_2D)) {
		cout << "Failed to initialize texture." << endl;
	}
	gameObjects[2].addShaders("shaders/tex2D.vert", "shaders/tex2D.frag");

	//make skybox TODO: put all in a method?
	vector<string> skyboxFiles = {
		"textures/ame_ash/ashcanyon_rt.tga",
		"textures/ame_ash/ashcanyon_lf.tga",
		"textures/ame_ash/ashcanyon_up.tga",
		"textures/ame_ash/ashcanyon_dn.tga",
		"textures/ame_ash/ashcanyon_bk.tga",		
		"textures/ame_ash/ashcanyon_ft.tga",
	};

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

	string musicFile= "music/BTS.wav";
	if (!audio.InitMusic(musicFile.c_str())) {
		cout << "Failed to load music." << endl;
	}

	if (!audio.PlayMusic()) {
		cout << "Failed to play music" << endl;
	}

	for (int i = 0; i < gameObjects.size(); i++) {
		if (!gameObjects[i].initBuffers()) {
			cout << "Could not initialize buffers for game object " << i << endl;
		}
	}
	camera = &testCams[camIndex];
	currentGEO = &gameObjects[geoIndex];
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		clearScreen();

		//render all game objects to screen
		for (int i = 0; i < gameObjects.size(); i++) {
			RenderGEO(&gameObjects[i]);
		}
		//render skybox
		RenderGEO(&skybox);
		RenderGEO(&plane);
		glfwSwapBuffers(window);

    AlternKeyCallback(window);
		glfwPollEvents();
	}

	// clean up allocated resources before exits
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i].shutdown();
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
/*
//Loads all meshes from directory
//NOTE: We don't really need this, it's overkill for what we need.
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

				//GEO geo;
				//geo.setFilename(entity->d_name);
				
				gameObjects.emplace_back();
				gameObjects.at(gameObjects.size()-1).setFilename(entity->d_name);

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
}*/

