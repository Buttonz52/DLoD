#include "Game/main.h"
#include "Physics\PhysXMain.h"

using namespace std;


void clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

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
    case GLFW_KEY_P:
      audio.PausePlay();
      break;

    case GLFW_KEY_E:
      audio.PlaySfx(audio.horn, MIX_MAX_VOLUME,3);
      break;

	case GLFW_KEY_N:
		audio.PlaySfx(audio.chicken, MIX_MAX_VOLUME,3);
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

  camera->setAsp((float)width / (float)height);
}

// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	skyboxIndex = 0;
	arenaIndex = 0;
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
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Get the desktop resolution.

	window = glfwCreateWindow(width, height, "Derby League of Destruction",0,0);

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
	// Enable blending (for transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);


	if (!audio.Init()) {
		cout << "Failed to init audio." << endl;
	}

	int numPlayers;
	while (!glfwWindowShouldClose(window)) {
		//Initialize "Loading screen"
		TitleScreen ts;

		ScreenOverlay loadBkgrd;
		if (ts.DisplayTitle(window, &testController, &audio, skyboxIndex, arenaIndex, &humanVehicleChoice, numPlayers)) {
			ts.Destroy();
			audio.FreeMusic();

			cout << "num players: " << numPlayers << endl;
			InitializeLoadScreen(&loadBkgrd);
			////init music
			if (!audio.InitMusic(mainMusic.c_str())) {
				cout << "Failed to load music." << endl;
			}
			glClearColor(0.0f, 51.f/255.f, 102.f/255.f, 1.0f);
			//enable depth buffer testing
			glEnable(GL_DEPTH_TEST);

			getSpawnPoints();

			PrintDirections();

			loadBkgrd.Destroy();
			//glfwSetWindowPos(window, 0,0);
			Game game(window, audio, skyboxFilePathnames[skyboxIndex], arenaObjFilenames[arenaIndex], starObjFilenames[arenaIndex], arenaMapFilenames[arenaIndex],&humanVehicleChoice, numPlayers, spawnPoints);

			if (!audio.PlayMusic()) {
				cout << "Failed to play music" << endl;
			}

			if (!game.start()) {
				glfwSetWindowShouldClose(window, true);
			}
			humanVehicleChoice.clear();
			audio.FreeMusic();
			ts.Destroy();

		}

		else {
			glfwSetWindowShouldClose(window, true);
			ts.Destroy();
		}
	}

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

void InitializeLoadScreen(ScreenOverlay *loadBkgrd) {
	//		ScreenOverlay loadBkgrd;
	if (!loadBkgrd->initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
		cout << "Failed to init loadBkgrnd." << endl;
	}

	loadBkgrd->InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	if (!loadBkgrd->GenerateSquareVertices(1, 1, vec3(0))) {
		cout << "Failed to initialize screen overlay." << endl;
	}
}

void getSpawnPoints()
{
	string meshname = starObjFilenames[arenaIndex];
	GEO *spawnGEO = new GEO();

	if (!spawnGEO->initMesh(meshname)) {
		cout << "Failed to init arena" << endl;
	}

	spawnPoints = spawnGEO->getMesh().vertices;

	for (int i = 0; i < spawnPoints.size(); i++)
	{
		spawnPoints[i] *= 30.f;
	}

}