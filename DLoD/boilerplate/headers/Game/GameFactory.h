#pragma once

#include "GEO/Player/AI.h"
#include "GEO/player/Human.h"
#include "Physics/PhysXMain.h"
#include "Game\ScreenOverlay.h"
#include "Game\Audio.h"
#include "Game\Timer.h"
#include "Game/GameHud.h"
#include "Game\PauseScreen.h"
#include "Game\EndGameScreen.h"



class GameFactory
{
protected:
	GEO* skybox;
	GEO* arena;
	Camera overheadCam, winningCam;	//camera of entire arena, camera of current winning car
	vector<Player*> players;
	vector<GEO*> physXObjects;
	GLFWwindow *window;
	string arenaTexFilename;
	string arenaMap;
	int width, height;
	int numPlayerScreens, pauseIdentifier, menuIndex;
	ScreenOverlay  switchCamText;
	Timer timer;
	bool pause, restart;


	GameState* gameState;

	// The name of this should change
	// vector of actors 
	vector<pair<Player*, int>> itemsToAdd;

	PhysXMain physX;
	Audio audio;

	GameHud gameHud;
	vec3 lightSource = vec3(0.f, 100.f, 0.f);

	bool gameOver = false;

	void ResizeViewport(const int index, const int numHumans, const int width, const int height);
	void goToEndGameState(const vector<string>& names, const vector<vec3>& colours);
	void goToGamePausedState();

	//TODO: Update these two functions so that they take in a vector of strings insted of 3 strings (make the thing look nicer)
	virtual void UpdateHudInfo(Player *player, mat4 & projectionMatrix, mat4 & viewMatrix, vector<string> &strings, vec3 & vColour, bool & canLayTrap);

	virtual void UpdateHudInfoEmpty(const vector <Player*> players, const int &i, mat4 &projectionMatrix, mat4 &viewMatrix, Camera &winningCam, Camera &overheadCam, vector<string> &strings,vec3 &vColour, const int &camIndex);

	void calculateRank(vector<string>& loserNames, vector<vec3>& loserColours, vector<clock_t>& ToDs);

	std::stringstream formatRank(Player * p, const int & numPlayerScreens, const int &index);

	void initSkyBox(const string &pathname);
	GEO* initArena(const string &texfilename, const string &objfilename);
	void initVehicle(Vehicle * v, int type);
	void initItem(Item * item, const PxVec3 & velocity);

	virtual void gameLoop();
public:
	virtual bool start();
	GameFactory();
	~GameFactory();
	GameFactory(GLFWwindow * w, Audio & audio, const string & skyboxFilepath, const string & arenaFilepath, const string & starObjFilename, const string & arenaMapFile, const vector<int>& humanVehicleChoice, const int & numPlayers, const vector<vec3>& spawnPoints, const vector<vec3>& itemSpawnPoints);
};

