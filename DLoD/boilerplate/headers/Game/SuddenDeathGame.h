#pragma once
#include "Game\GameFactory.h"

class SuddenDeathGame: public GameFactory
{
private: 

public:
	SuddenDeathGame(GLFWwindow * w, Audio & audio, const string & skyboxFilepath, const string & arenaFilepath, const string & starObjFilename, const string & arenaMapFile, const vector<int> &humanVehicleChoice, const int & numPlayers, const vector<vec3>& spawnPoints, const vector<vec3>& itemSpawnPoints);
	~SuddenDeathGame();
	void UpdateHudInfo(Player *player, mat4 & projectionMatrix, mat4 & viewMatrix, vector<string> &strings, vec3 & vColour, bool & canLayTrap);

	//virtual void UpdateHudInfoEmpty(const vector <Player*> players, const int &i, mat4 &projectionMatrix, mat4 &viewMatrix, Camera &winningCam, Camera &overheadCam, string &healthStr, string &armourStr, string &velocityStr, vec3 &vColour, const int &camIndex);
	void UpdateHudInfoEmpty(const vector <Player*> players, const int &i, mat4 &projectionMatrix, mat4 &viewMatrix, Camera &winningCam, Camera &overheadCam, vector<string> &strings, vec3 &vColour, const int &camIndex);

	void gameLoop();
	bool start();

};

