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
	// Enable blending (for transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
	glClearColor(0.0f, 51.f / 255.f, 102.f / 255.f, 1.0f);


	int numPlayers;
	while (!glfwWindowShouldClose(window)) {
		if (!audio.Init()) {
			cout << "Failed to init audio." << endl;
		}
		//Initialize "Loading screen"
		TitleScreen ts;
		humanVehicleChoice.clear();
		glDisable(GL_DEPTH_TEST);
		if (ts.DisplayTitle(window, &testController, &audio, skyboxIndex, arenaIndex, &humanVehicleChoice, numPlayers)) {
			ts.Destroy();
			audio.FreeMusic();

			cout << "num players: " << numPlayers << endl;
			////init music
			if (!audio.InitMusic(mainMusic.c_str())) {
				cout << "Failed to load music." << endl;
			}
			//enable depth buffer testing
			glEnable(GL_DEPTH_TEST);

			getSpawnPoints();
			getItemSpawnPoints();

			PrintDirections();

			//glfwSetWindowPos(window, 0,0);
			Game game(window, audio, skyboxFilePathnames[skyboxIndex], arenaObjFilenames[arenaIndex], starObjFilenames[arenaIndex], arenaMapFilenames[arenaIndex],&humanVehicleChoice, numPlayers, spawnPoints, itemSpawnPoints);

			if (!audio.PlayMusic()) {
				cout << "Failed to play music" << endl;
			}

			if (!game.start()) {
				glfwSetWindowShouldClose(window, true);
			}
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

void getSpawnPoints()
{
	string meshname = spawnObjFilenames[arenaIndex];
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

void getItemSpawnPoints()
{
	string meshname = itemSpawnObjFilenames[arenaIndex];
	GEO *spawnGEO = new GEO();

	if (!spawnGEO->initMesh(meshname)) {
		cout << "Failed to init arena" << endl;
	}

	itemSpawnPoints = spawnGEO->getMesh().vertices;

	for (int i = 0; i < itemSpawnPoints.size(); i++)
	{
		itemSpawnPoints[i] *= 30.f;
	}
}