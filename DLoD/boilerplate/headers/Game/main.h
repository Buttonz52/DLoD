/*Note: cpp files aren't in classes, probably should do that. */
#ifndef MAIN_H
#define MAIN_H

#include "Utility.h"
#include "Audio.h"
#include "Controller/XboxController.h"
#include "TitleScreen.h"
//#include "GEO/Shadow.h"
#include "GameFactory.h"
#include "TimedGame.h"
#include "SuddenDeathGame.h"

using namespace std;

// --------------------------------------------------------------------------
// Variables
vector<int> humanVehicleChoice;
vector<vec3> spawnPoints;
vector<vec3> itemSpawnPoints;
//test cameras

int camIndex = 0, geoIndex = 0, modeIndex=0; 

int width = 1920, height = 1080;
int numPlayers = 1;

mat4 winRatio = mat4(1.f);

Audio audio;

string mainMusic = "music/TimeLapse.wav";

int skyboxIndex, arenaIndex;
//skybox file paths
vector<string> skyboxFilePathnames, arenaObjFilenames, itemSpawnObjFilenames;
//vector<string> skyboxFilePathnames;/* = {
//	"textures/ely_cloudtop/",
//	"textures/nec_hell/",
//	"textures/mp_deviltooth/",
//	"textures/mp_emerald/"
//};*/
//
////arena obj files
//vector <string> arenaObjFilenames;/* = {
//	"arenas/bowl.obj",
//	"arenas/devil.obj",
//	"arenas/wrap.obj" */

//
//vector<string> spawnObjFilenames = {
//	"spawns/spawn.obj",
//};
//
//vector<string> itemSpawnObjFilenames = {
//	"itemspawns/bowlitemspawn.obj",
//	"itemspawns/devilitemspawn.obj",
//	"itemspawns/wrapitemspawn.obj",
//	"itemspawns/bowlitemspawn.obj",
//	"itemspawns/bowlitemspawn.obj"
//};

vector<string> starObjFilenames = {
	"starNodes/bowl.obj",
	"starNodes/devil.obj",
	"starNodes/wrap.obj",
	"starNodes/bowl.obj",
	"starNodes/bowl.obj"
};

//arena radar
vector <string> arenaMapFilenames = {
	"textures/arenaImgs/radar/circle.png",
	"textures/arenaImgs/radar/circle.png",
	"textures/arenaImgs/radar/circle.png",
	"textures/arenaImgs/radar/circle.png",
	"textures/arenaImgs/radar/circle.png"
};
//Audio music;

// OpenGL utility and support function prototypes
void getSpawnPoints();
void getItemSpawnPoints();

//PhysX object
PhysXMain PhysX;
GEO arena , skybox;
Camera cam;
int prevArenaIndex, prevSkyboxIndex;
//controller
XboxController controller(1);

// --------------------------------------------------------------------------
#endif