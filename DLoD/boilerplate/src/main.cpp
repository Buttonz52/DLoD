#include "Game/main.h"

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
      
	case GLFW_KEY_P:
		audio.PausePlay();
		break;

	case GLFW_KEY_S:
		audio.PlaySfx(audio.horn);
		break;

    case GLFW_KEY_I:
      curPlayer.playerCam.translate3D(vec3(0,0.2,0));
      break;
      
    case GLFW_KEY_K:
		curPlayer.playerCam.translate3D(vec3(0,-0.2,0));
      break;
      
    case GLFW_KEY_J:
		curPlayer.playerCam.translate3D(vec3(-0.2,0,0));
      break;
      
    case GLFW_KEY_L:
		curPlayer.playerCam.translate3D(vec3(0.2,0,0));
      break;

    case GLFW_KEY_RIGHT:
		curPlayer.playerCam.incrementAzu(-M_PI/180);
      break;
      
    case GLFW_KEY_LEFT:
		curPlayer.playerCam.incrementAzu(M_PI/180);
      break;
      
    case GLFW_KEY_UP:
		curPlayer.playerCam.incrementAlt(-M_PI/180);
      break;
      
    case GLFW_KEY_DOWN:
		curPlayer.playerCam.incrementAlt(M_PI/180);
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
		curPlayer.playerCam.incrementAzu(dx * 0.005f);
		curPlayer.playerCam.incrementAlt(dy * 0.005f);
	}
  
	mouse_old_x = x;
	mouse_old_y = y;
}

//handles mouse scroll
void scroll_callback(GLFWwindow* window, double x, double y)
{
	curPlayer.playerCam.incrementRadius(y / 2);
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

	if (!audio.InitMusic("music/music.wav")) {
		cout << "Failed to load music." << endl;
	}
	if (!audio.PlayMusic()) {
		cout << "Failed to play music" << endl;
	}

	// call function to load and compile shader programs
	int numObjFiles = LoadAllObjFiles("models");
	cout << "Num obj files: " << numObjFiles << endl;

	/*Plane plane;

	vec3 planecol = vec3(1.f, 0.f, 0.f);
	if (!plane.CreatePlane(10.0, &planecol, 1)) {
		cout << "Failed to create plane." << endl;
	}
	plane.mesh.SetScale(1.f);
	plane.SetPosition(vec3(0, -0.3f, 0.f));
	plane.mesh.vertex = "shaders/plane.vert";
	plane.mesh.fragment = "shaders/plane.frag";

	plane.mesh.texture.InitializeTexture("textures/images/zebra.png", GL_TEXTURE_2D);
	*/

	//plane.mesh.shader.InitializeShaders(plane.mesh.vertex, plane.mesh.fragment);
	//if (!plane.mesh.Initialize()) {
	//	cout << "ERROR: Could not initialize mesh." << endl;
	//}

		//p.RenderMesh(&winRatio, &_lightSource, width, height);

	//Random testing purposes
	vec3 grcol(0.0, 1.0, 0.0);
	meshes[2].AddColour(&grcol);

	//meshes[2].texture.InitializeTexture("textures/images/zebra.png", GL_TEXTURE_2D);
	meshes[0].AddTexture("textures/images/zebra.png");
	meshes[1].AddTexture("textures/images/zebra.png");
	meshes[2].AddTexture("textures/images/zebra.png");
	Player p;
	p.vehicle.mesh = meshes[1];
	p.vehicle.mesh.shader.InitializeShaders(p.vehicle.mesh.vertex, p.vehicle.mesh.fragment);

	if (!p.vehicle.mesh.Initialize()) {
		cout << "ERROR: Could not initialize mesh." << endl;
	}
	curPlayer = p;
	while (!glfwWindowShouldClose(window))
	{
		//plane.cam = curPlayer.playerCam;	//just like this for now because I'm lazy 
		//plane.RenderPlane(winRatio, _lightSource, width, height);

		// call function to draw our scene
		/*
		if(g_play){
		t += dt;
		animateQuad(t);
		}
		*/
		//Just renders first mesh for now.
		//meshes[2].texture.BindTexture(meshes[2].program, GL_TEXTURE_2D, "sampler");

		p.playerCam = camera;	//This is pretty hacky...
		p.RenderMesh(&winRatio, &_lightSource, width, height);

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
				string s(pathname);
				s += "/" + string(entity->d_name);

				//Add mesh to mesh vector
				AddMesh(&string(s), &string (entity->d_name));
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
void AddMesh(const string *pathname, const string * filename) {
	Mesh mesh;
	//Get all information for mesh

	mesh.ReadMesh(*pathname);

	//Add colour for the moment; this can be taken out
	//or colour changed/colour added to obj files and 
	//not here
	vec3 red(1.f, 0.f, 0.f);
	mesh.AddColour(&red);
	string vertex;
	string frag;

	size_t endpos = string(*filename).find(".obj");
	string shadername = string(*filename).substr(0, endpos);

	string shaderpath = "shaders/";
	mesh.vertex = shaderpath + shadername + ".vert";
	mesh.fragment = shaderpath + shadername + ".frag";

	//This gives an invalid valie.
	//mesh.program = mesh.shader.InitializeShaders(vertex, frag);

	meshes.push_back(mesh);
	cout << "Loaded " << *pathname << endl;
}
