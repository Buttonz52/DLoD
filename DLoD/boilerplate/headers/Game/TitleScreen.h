#pragma once
#include "Utility.h"
#include "GameEngine/Mesh.h"
#include "ScreenOverlay.h"
#include "Controller/XboxController.h"
#include "Audio.h"
#include "Game\timer.h"
#include "Title\Screen.h"
#include "Title\MainScreen.h"
#include "Title\RulesScreen.h"
#include "Title\MultiplayerScreen.h"
#include "Title\ModeScreen.h"
#include "Title\CarScreen.h"
#include "Title\ArenaScreen.h"
#include "Title\SkyboxScreen.h"


class TitleScreen
{
public:
	TitleScreen();
	~TitleScreen();

	void Render();
	bool Initialize(GLFWwindow *, XboxController *, Audio *);
	int Run(vector<int> &humanVehicleChoice, int &numPlayers, int & mode);
	void Destroy();

private:
	Screen *currentScreen;
	bool isQuit, isStart;

	int pauseTime, clickChannel, backChannel, revChannel1, revChannel2,
		vehicleIndex;
	string titleMusic = "music/Faded_intro.wav";

	vec3 selectColour, pressColour, prevColour;
	GLFWwindow *window;
	XboxController *controller;
	vector <XboxController> controllers;
	Audio *audio;

	ScreenOverlay background;

	Timer timer; 
	
	void pressStart(Audio *audio);
	void pressQuit();
};
