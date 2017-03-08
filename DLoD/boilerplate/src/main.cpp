#include "Game/main.h"
#include "Physics\PhysXMain.h"

using namespace std;


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

  camera->setAsp((float)width / (float)height);
}

// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	skyboxIndex = 0;
	arenaIndex = 0;
	humanVehicleChoice = 0;
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

	if (!audio.Init()) {
		cout << "Failed to init audio." << endl;
	}
	////init music
	if (!audio.InitMusic(mainMusic.c_str())) {
		cout << "Failed to load music." << endl;
	}

	//Initialize "Loading screen"
	TitleScreen ts;

	ScreenOverlay loadBkgrd, loadWidget;
	//while (!glfwWindowShouldClose(window)) {
		if (ts.DisplayTitle(window, &testController, &audio, skyboxIndex, arenaIndex, humanVehicleChoice)) {
			ts.Destroy();
			InitializeLoadScreen(&loadBkgrd, &loadWidget);

			updateLoadBar(window, loadBkgrd, loadWidget, loadWidget.updateFactor);

			updateLoadBar(window, loadBkgrd, loadWidget, loadWidget.updateFactor);

			glEnable(GL_DEPTH_TEST);

			PrintDirections();

			updateLoadBar(window, loadBkgrd, loadWidget, loadWidget.updateFactor);
			loadBkgrd.Destroy();
			loadWidget.Destroy();

			Game game(window, audio, skyboxFilePathnames[skyboxIndex], arenaObjFilenames[arenaIndex], humanVehicleChoice);

			if (!audio.PlayMusic()) {
				cout << "Failed to play music" << endl;
			}

			game.start();
		}
		else {
			glfwSetWindowShouldClose(window, true);
			ts.Destroy();
		}
//	}
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

void InitializeLoadScreen(ScreenOverlay *loadBkgrd, ScreenOverlay *loadWidget) {
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
}
