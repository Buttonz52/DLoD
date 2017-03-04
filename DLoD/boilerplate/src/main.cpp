#include "Game/main.h"
#include "Physics\PhysXMain.h"

using namespace std;
bool paused = false;

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
	//camIndex >= testCams.size()-1 ? camIndex = 0 : camIndex++;	//if statement: increment/reset camIndex 
	camIndex >= players.size() - 1 ? camIndex = 0 : camIndex++;	//if statement: increment/reset camIndex 

	//camera = &testCams[camIndex];		//new camera
	currentPlayer = players[camIndex];
	//testing purposes only
	//vec3 *c = camera->getCenter();		//just printing out camera center for testing purposes, delete later
//	cout << c->x << " " << c->y << " " << c->z << endl;
}

void clearScreen() {
	// clear screen to a dark grey colour;
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void updateLoadBar(GLFWwindow *window, ScreenOverlay &loadBkgrd, ScreenOverlay &loadWidget, const float &incr) {
	clearScreen();
	loadBkgrd.Render(GL_TRIANGLE_STRIP);	//render "loading screen"

											//Well, this is kind of a weird way to implement this, change later.
											//loadWidget.setPosition(loadWidget.getPosition() + vec3(0.05, 0, 0));
	loadWidget.setScale(loadWidget.getScale() + vec3(incr, 0, 0));	//makes larger
	//loadWidget.setPosition(loadWidget.getPosition() + vec3(incr, 0, 0));
	loadWidget.Render(GL_TRIANGLE_STRIP);	//render widget; this is just for testing at the moment
											//Sleep(200); //Just to slow down the animation a bit
	glfwSwapBuffers(window);	//need this to output to screen
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

  //Movement of the GEOs with keyboard if controller not connected
  if (!testController.Connected())
  {
	  if (!currentVehicle->isDead()) {
		  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		  {
			  currentVehicle->accelerate(1);
		  }
		  else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		  {
			  currentVehicle->decelerate(1);
		  }
		  else {
			  currentVehicle->brake(8000.f);
		  }
		  state = glfwGetKey(window, GLFW_KEY_LEFT);
		  if (state == GLFW_PRESS)
		  {
			  currentVehicle->turn(-1);
		  }
		  state = glfwGetKey(window, GLFW_KEY_RIGHT);
		  if (state == GLFW_PRESS)
		  {
			  currentVehicle->turn(1);
		  }
	  }
	  else {
		  currentVehicle->brake(8000.f);
	  }
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

	case GLFW_KEY_SPACE:
    paused = !paused;
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
	float turn;
	if (!currentVehicle->isDead()) {
		if (testController.RightTrigger() != 0)
		{
			currentVehicle->accelerate(testController.RightTrigger());
		}

		else if (testController.LeftTrigger() != 0)
		{
			currentVehicle->decelerate(testController.LeftTrigger());
		}
		else
		{
			currentVehicle->brake(1000.f);
		}
		if (testController.LeftStick_X() > -0.25 && testController.LeftStick_X() < 0.25)
			turn = 0.0;
		else
			turn = testController.LeftStick_X();

		currentVehicle->turn(turn);
	}
	else {	//just stop
		currentVehicle->brake(1000.0);
	}

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
	camera = &testCams[camIndex];

	//init music
	if (!audio.InitMusic(mainMusic.c_str())) {
		cout << "Failed to load music." << endl;
	}

	//Initialize "Loading screen"
	TitleScreen ts;
	AI dummyAI;
	Human p1;
	mat4 _proj, _view;
	GEO plane, skybox;
	ScreenOverlay logo, loadBkgrd, loadWidget;
	int frameCtr;

	bool playGame = ts.Display(window, &testController);
	if (playGame) {
		InitializeLoadScreen(&loadBkgrd, &loadWidget, &logo);

		updateLoadBar(window, loadBkgrd, loadWidget, loadWidget.updateFactor);

		//initialize PhysX
		PhysX.init();

		updateLoadBar(window, loadBkgrd, loadWidget, loadWidget.updateFactor);

		if (!audio.PlayMusic()) {
			cout << "Failed to play music" << endl;
		}

		//human and AI instead of just using vehicle

		dummyAI.vehicle = new Vehicle();
		p1.vehicle = new Vehicle();
		dummyAI.vehicle->setPosition(vec3(30, 0, 30));
		initVehicle(p1.vehicle);
		initVehicle(dummyAI.vehicle);

		players.push_back(&p1);
		players.push_back(&dummyAI);

		plane = initGroundPlane();
		skybox = initSkyBox();

		currentVehicle = p1.vehicle;
		currentPlayer = &p1;
		glEnable(GL_DEPTH_TEST);

		frameCtr = 0;
		PrintDirections();

		updateLoadBar(window, loadBkgrd, loadWidget, loadWidget.updateFactor);
		loadBkgrd.Destroy();
		loadWidget.Destroy();
	}
	while (!glfwWindowShouldClose(window))
	{
		clearScreen();
		//input

		PhysX.stepPhysics(true, gameObjects);
		
		//update

		camera->followVehicle(currentPlayer->vehicle);
		//Check if AI is dead.  If so, can't move. Else, moves.
		dummyAI.isDead() ==false ? dummyAI.driveTo(p1.vehicle->getModelMatrix()[3]) : dummyAI.vehicle->brake(10000.0);
		
		if(frameCtr % 60 == 0)
		{
			cout << "Vehicle speed: " << p1.vehicle->physXVehicle->computeForwardSpeed() << "\tVehicle mass: " << p1.vehicle->physXVehicle->getRigidDynamicActor()->getMass() << endl;
			cout << "Player 1 HP: " << p1.vehicle->getHealth() << "\tDummyCar HP: " << dummyAI.vehicle->getHealth() << endl << endl;
		}
		frameCtr++;
		
		_proj = winRatio * camera->calculateProjectionMatrix((float)width / (float)height);
		_view = camera->calculateViewMatrix();

		p1.vehicle->Render(p1.vehicle->getShader(),_view, _proj, _lightSource);
		dummyAI.vehicle->Render(dummyAI.vehicle->getShader(),_view, _proj, _lightSource);
		skybox.Render(skybox.getShader(), _view, _proj, _lightSource);
		plane.Render(plane.getShader(),_view, _proj, _lightSource);
		logo.Render(GL_TRIANGLE_STRIP);	//render logo
		
		
		glfwSwapBuffers(window);

    if (testController.Connected())
      GetControllerInput();

    AlternKeyCallback(window);
    glfwPollEvents();
	}

	p1.vehicle->shutdown();
	dummyAI.vehicle->shutdown();
	skybox.shutdown();
	plane.shutdown();
	logo.Destroy();
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

void initVehicle(Vehicle* v)
{
  v->setScale(vec3(0.8));

	v->setFilename("teapot.obj");	//alive mesh
	if (!v->initMesh("cube.obj")) {	//dead mesh
		cout << "Failed to initialize mesh." << endl;
	}
	v->addShaders("shaders/toon.vert", "shaders/toon.frag");


	if (!v->initBuffers()) {
		cout << "Could not initialize buffers for game object " << v->getFilename() << endl;
	}

	PhysX.initVehicle(v);

	gameObjects.push_back(v);
}

GEO initGroundPlane()
{
	GEO plane;
	plane.setFilename("plane.obj");
	if (!plane.initMesh("plane.obj")) {
		cout << "Failed to initialize mesh." << endl;
	}
	plane.setScale(vec3(150.f));
	if (!plane.initTexture("textures/ground.png", GL_TEXTURE_2D)) {
		cout << "Failed to initialize plane." << endl;
	}
	plane.addShaders("shaders/tex2D.vert", "shaders/tex2D.frag");
	plane.setPosition(vec3(0, -0.7, 0));

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
	if (!skybox.initMesh("cube.obj")) {
		cout << "Failed to initialize mesh." << endl;
	}
	//scale cube large
	skybox.setScale(vec3(500.0));
	if (!skybox.initSkybox(skyboxFiles)) {
		cout << "Failed to initialize skybox." << endl;
	}
	skybox.addShaders("shaders/skybox.vert", "shaders/skybox.frag");

	return skybox;
}

void InitializeLoadScreen(ScreenOverlay *loadBkgrd, ScreenOverlay *loadWidget, ScreenOverlay *logo) {
	//		ScreenOverlay loadBkgrd;
	if (!loadBkgrd->initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
		cout << "Failed to init loadBkgrnd." << endl;
	}

	loadBkgrd->InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!loadBkgrd->GenerateSquareVertices(1, 1, vec3(0))) {
		cout << "Failed to initialize screen overlay." << endl;
	}
	
	//Initialize load widget; this is just a test to see that multiple things render

	loadWidget->InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!loadWidget->GenerateSquareVertices(0.1, 0.1, vec3(1, 0, 0))) {
		cout << "Failed to initialize screen overlay." << endl;
	}
	//faaaar left
	loadWidget->setPosition(vec3(-4, -0.75, 0));

	//Not working properly at the moment, otherwise, move to this code insted of the above code.
	//if (!loadBkgrd->InitQuad("textures/DLoDLogo.png",
	//	"shaders/screenOverlay.vert",
	//	"shaders/screenOverlay.frag",
	//	1.f,
	//	1.f,
	//	vec3(0))) {
	//	cout << "Failed to init load bkgrnd." << endl;
	//}
	////Initialize load widget; this is just a test to see that multiple things render
	//if (!loadWidget->InitQuad("",
	//	"shaders/screenOverlay.vert",
	//	"shaders/screenOverlay.frag",
	//	0.1,
	//	0.1,
	//	vec3(1, 0, 0))) {
	//	cout << "Failed to init load load bar." << endl;
	//}

	//Initialize logo in top right corner 
	if (!logo->initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
		cout << "Failed to init texture." << endl;
	}

	logo->InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!logo->GenerateSquareVertices(0.1, 0.1, vec3(0))) {
		cout << "Failed to initialize screen overlay." << endl;
	}
	logo->setPosition(vec3(0.9f, 0.9f, 0));
}
