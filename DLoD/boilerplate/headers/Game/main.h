/*Note: cpp files aren't in classes, probably should do that. */
#ifndef MAIN_H
#define MAIN_H

#include "../GameEngine/Mesh.h"
#include "../GameEngine/Shader.h"
#include "Utility.h"
#include "../GameEngine/Texture.h"
#include "../GEO/Camera.h"
#include "../GEO/Player/AI.h"
#include "../GEO/player/Human.h"
#include "Audio.h"
#include "../Physics/PhysXMain.h"
#include "Controller/XboxController.h"
#include "ScreenOverlay.h"
#include "TitleScreen.h"
#include "../GEO/Shadow.h"
#include "../GameEngine/Text.h"
#include "Game.h"

using namespace std;

// --------------------------------------------------------------------------
// Variables
vector<vec3> _colours;
vector<vec3> _vertices;
vector<vec2> _uv;
vector<GLushort> _faces;
vector<vec3> _normals;
vector<GEO *> gameObjects;

vector<Player*> players;
Player * currentPlayer;
//test cameras
vector<Camera> testCams = {	//just objects for now, make pointers or smth later
	Camera(vec3(0, 0, 0)),	//first teapot
	Camera(vec3(-3, 0, -10)),	//second teapot
	Camera(vec3(0, 10, -10)),	//random
	Camera(vec3(-4, -4, -5), vec3(-3, 0, -10))	//random
};
int camIndex = 0, geoIndex = 0;	//index of test cameras

int width = 1920, height = 1080;

double mouse_old_x, mouse_old_y;
float _translate_z = 1.0;
float _rotate_x = 0.0;
float _rotate_y = 0.0;

mat4 _view;
mat4 _projection;
mat4 winRatio = mat4(1.f);

Camera *camera;
Audio audio;
Vehicle *currentVehicle;
vec3 _lightSource = vec3(0.f, 100.f, 0.f);

//string mainMusic= "music/TimeLapse.wav";	//theme
string mainMusic = "music/Fluffing_a_duck.wav";
//Audio music;

// OpenGL utility and support function prototypes
void PrintDirections();

// Mesh functions
int LoadAllObjFiles(const char *pathname);	
void InitializeLoadScreen(ScreenOverlay *loadBkgrd, ScreenOverlay *loadWidget);
//init GEO functions
GEO* initCube();
void initVehicle(Vehicle* v);
GEO initGroundPlane();
GEO initSkyBox();

//PhysX object
PhysXMain PhysX;

//controller
XboxController testController(1);

// --------------------------------------------------------------------------
#endif