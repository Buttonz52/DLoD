#pragma once
#include "Game\GameFactory.h"


class TimedGame: public GameFactory
{
public:
	//gameHud.setMode(GameHud::time);
	~TimedGame();
	TimedGame(GLFWwindow * w, Audio & audio, const string & skyboxFilepath, const string & arenaFilepath, const string & starObjFilename, const string & arenaMapFile, const vector<int>* humanVehicleChoice, const int & numPlayers, const vector<vec3>& spawnPoints, const vector<vec3>& itemSpawnPoints);
	void UpdateHudInfo(Player * player, mat4 & projectionMatrix, mat4 & viewMatrix, vector<string>& strings, vec3 & vColour, bool & canLayTrap);
	void UpdateHudInfoEmpty(const vector<Player*> players, const int & i, mat4 & projectionMatrix, mat4 & viewMatrix, Camera & winningCam, Camera & overheadCam, vector<string>& strings, vec3 & vColour, const int & camIndex);
	void gameLoop();
	bool start();
private: 
	int endGameTickNum;
	int currentTicks;
	Timer gameTimer;
	bool timeOver;
};

