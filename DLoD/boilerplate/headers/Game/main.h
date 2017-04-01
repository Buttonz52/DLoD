/*Note: cpp files aren't in classes, probably should do that. */
#ifndef MAIN_H
#define MAIN_H

#include "Utility.h"
#include "Audio.h"
#include "Controller/XboxController.h"
#include "TitleScreen.h"
//#include "GEO/Shadow.h"
#include "Game.h"

using namespace std;

// --------------------------------------------------------------------------
// Variables
vector<int> humanVehicleChoice;
vector<vec3> spawnPoints;
//test cameras

int camIndex = 0, geoIndex = 0;	//index of test cameras

int width = 1920, height = 1080;
int numPlayers = 1;

mat4 winRatio = mat4(1.f);

Audio audio;

string mainMusic = "music/TimeLapse.wav";

int skyboxIndex, arenaIndex;
//skybox file paths
vector<string> skyboxFilePathnames = {
	"textures/ely_cloudtop/",
	"textures/nec_hell/",
	"textures/mp_deviltooth/",
	"textures/mp_emerald/"
};

//arena obj files
vector <string> arenaObjFilenames = {
	"arenas/test.obj",
	"arenas/devil.obj",
	"arenas/wrap.obj"
};

vector<string> starObjFilenames = {

	"spawns/spawn.obj",
	"spawns/spawn.obj",
	"spawns/spawn.obj"
};

//arena radar
vector <string> arenaMapFilenames = {
	"textures/arenaImgs/radar/circle.png",
	"textures/arenaImgs/radar/circle.png",
	"textures/arenaImgs/radar/circle.png"
};
//Audio music;

// OpenGL utility and support function prototypes
void PrintDirections();
void getSpawnPoints();
// Mesh functions

//PhysX object
PhysXMain PhysX;

//controller
XboxController testController(1);

// --------------------------------------------------------------------------
#endif