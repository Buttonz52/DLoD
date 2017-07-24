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

void InitSkybox() {
	string pathname = skyboxFilePathnames[skyboxIndex];
	string skyboxConfigFile = pathname + "skyboxFile.txt";
	ifstream f_stream(skyboxConfigFile);
	if (!f_stream.is_open()) {
		//	cout << "Error loading file " << skyboxConfigFile << endl;
		return;
	}
	//load the file
	string line;
	size_t found;
	vector<string> skyboxFiles;
	//	cout << "Loading file " << skyboxConfigFile << endl;
	while (getline(f_stream, line)) {
		//If there is a #, then there is a new celestial body to be added
		if (line.length() == 0) {
			continue;
		}
		if ((found = line.find("bump")) != string::npos) {
			//arenaBumpmap = line;
			continue;
		}
		skyboxFiles.push_back(pathname + line);
	}
	
	skybox.setColour(vec3(0, 0, 0));
	if (!skybox.initMesh("cube.obj")) {
		cout << "Failed to initialize skybox mesh." << endl;
	}
	//scale cube large
	skybox.setScale(vec3(1500.0));
	skybox.updateModelMatrix();
	if (!skybox.initSkybox(skyboxFiles)) {
		cout << "Failed to initialize skybox texture." << endl;
	}
	skybox.addShaders("shaders/skybox.vert", "shaders/skybox.frag");
}
void InitArena() {
	prevArenaIndex = arenaIndex;
	arena.setColour(vec3(1, 0, 0));

	if (!arena.initMesh(arenaObjFilenames[arenaIndex])) {
		cout << "Failed to init arena" << endl;
	}
	arena.setEnvironmentMap(skybox.getTexture());
	arena.setReflectance(0.1);
	arena.setExposure(0.7);
	if (!arena.initTexture("textures/gold.png", GL_TEXTURE_2D)) {
		//if (!arena->initTexture("textures/ground.png", GL_TEXTURE_2D)) {

		cout << "Failed to initialize arena ground texture." << endl;
	}
	//arena.addShaders("shaders/tex2D.vert", "shaders/tex2D.frag");
	arena.addShaders("shaders/hdr.vert", "shaders/hdr.frag");

	arena.setScale(vec3(30.f));
	arena.setPosition(vec3(0, 0, 0));
	arena.updateModelMatrix();
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
      audio.PlaySfx("horn", MIX_MAX_VOLUME,menu);
      break;

	case GLFW_KEY_N:
		audio.PlaySfx("chicken", MIX_MAX_VOLUME,menu);

	//case GLFW_KEY_ESCAPE:
	//	glfwSetWindowShouldClose(window, true);
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

bool InitializeTitleScreen(TitleScreen &ts, GLFWwindow *window, XboxController *x, Audio *a) {
	if (!ts.Initialize(window, x, a)) {
		cout << "Failed to initialize title screen." << endl;
		return false;
	}
	return true;
}
// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	skyboxIndex = 0;
	arenaIndex = 0;
	prevArenaIndex = 0;
	prevSkyboxIndex = 0;
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

	//fullscreen, put back in once done debugging 
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
 // GLFWmonitor* monitor = glfwGetPrimaryMonitor();
 // const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  //glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

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
	glClearColor(51.f / 255.f, 1.f, 153.f / 255.f, 1.0);


	int numPlayers;
	ScreenOverlay loadPage;
	loadPage.GenerateSquareVertices(1.f, 1.f, vec3(1.f));
	loadPage.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	if (!loadPage.InitTexture("textures/DLoDLogo.png", GL_TEXTURE_2D))
		std::cout << "Failed to initialize load page texture." << endl;
	loadPage.Render(GL_TRIANGLE_STRIP, vec3());
	glfwSwapBuffers(window);	//need this to output to screen
	if (!audio.Init()) {
		cout << "Failed to init audio." << endl;
	}
	if (!audio.InitAllSounds()) {
		cout << "Failed to init sounds:" << endl;
	}
	TitleScreen ts;
	if (!InitializeTitleScreen(ts, window, &controller, &audio))
		return -1;

	humanVehicleChoice.clear();
	InitSkybox();
	InitArena();
	vec3 light = vec3(100, 0, 0);
	//fix angle and radius of camera
	cam.setAlt(0);
	cam.setRadius(500);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		if (prevSkyboxIndex != skyboxIndex) {
			skybox.shutdown();
			InitSkybox();
		}
		if (prevArenaIndex != arenaIndex) {
			arena.shutdown();
			InitArena();
		}
		mat4 proj = cam.calculateProjectionMatrix();
		mat4 view = cam.calculateViewMatrix();
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//fix how arena and skybox are rendering
		arena.Render(view, proj, light);
		//glDisable(GL_CULL_FACE);

		skybox.Render(view, proj, light);
		cam.incrementAzu(0.01f);
		glDisable(GL_DEPTH_TEST);

		int ts_state = ts.Run(skyboxIndex, arenaIndex, humanVehicleChoice, numPlayers, modeIndex);
		if (ts_state != 0) {
			continue;
		}
		arena.shutdown();
		skybox.shutdown();
		ts.Destroy();
		audio.FreeMusic();

		////init music
		if (!audio.InitMusic(mainMusic.c_str())) {
			cout << "Failed to load music." << endl;
		}
		//enable depth buffer testing
		glEnable(GL_DEPTH_TEST);

		getSpawnPoints();
		getItemSpawnPoints();

		//glfwSetWindowPos(window, 0,0);
		GameFactory *game;
		switch (modeIndex) {

		case 0:
			game = new TimedGame(window, audio, skyboxFilePathnames[skyboxIndex], arenaObjFilenames[arenaIndex], starObjFilenames[arenaIndex], arenaMapFilenames[arenaIndex], humanVehicleChoice, numPlayers, spawnPoints, itemSpawnPoints);
			cout << "timed" << endl;
			break;
		case 1:
			cout << " death" << endl;
			game = new SuddenDeathGame(window, audio, skyboxFilePathnames[skyboxIndex], arenaObjFilenames[arenaIndex], starObjFilenames[arenaIndex], arenaMapFilenames[arenaIndex], humanVehicleChoice, numPlayers, spawnPoints, itemSpawnPoints);
			break;
		default:
			game = new GameFactory(window, audio, skyboxFilePathnames[skyboxIndex], arenaObjFilenames[arenaIndex], starObjFilenames[arenaIndex], arenaMapFilenames[arenaIndex], humanVehicleChoice, numPlayers, spawnPoints, itemSpawnPoints);
		}
		if (!audio.PlayMusic()) {
			cout << "Failed to play music" << endl;
		}
		//if no restart, then game is over
		if (!game->start()) {
			glfwSetWindowShouldClose(window, true);
		}
		else {
			if (!InitializeTitleScreen(ts, window, &controller, &audio))
				return -1;
			InitSkybox();
			InitArena();
		}
	}
	arena.shutdown();
	skybox.shutdown();
	ts.Destroy();
	glfwSetWindowShouldClose(window, true);
	
	loadPage.Destroy();
	audio.CleanUp();

	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

void getSpawnPoints()
{
	string meshname = spawnObjFilenames[arenaIndex];
	GEO *spawnGEO = new GEO();

	if (!spawnGEO->initMesh(meshname)) {
	//	cout << "Failed to init arena" << endl;
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
	//	cout << "Failed to init arena" << endl;
	}

	itemSpawnPoints = spawnGEO->getMesh().vertices;

	for (int i = 0; i < itemSpawnPoints.size(); i++)
	{
		itemSpawnPoints[i] *= 30.f;
	}
}
