#pragma once
#include <memory>
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
#include "Title\PlayButtonScreen.h"


using namespace glm;
class TitleScreen
{
public:
	TitleScreen();
	~TitleScreen();

	void resetScreen();

	void Render();
	int Run(vector<int>& humanVehicleChoice, int & numPlayers, int & mode, vec3 & newColour);
	bool Initialize(GLFWwindow *, XboxController *, Audio *);
	void UpdateColour(const vec3 & colour);
	void Destroy();

	void InitCarScreen(GLFWwindow * w, vector<XboxController>& c, Audio * a, const int & numPlayers,vec3 & co);
	void InitModeScreen(GLFWwindow * w, XboxController * c, Audio * a, vec3 & co);
	void InitMainScreen(GLFWwindow * w, XboxController * c, Audio * a, vec3 & co);
	void InitMultiplayerScreen(GLFWwindow * w, XboxController * c, Audio * a, vec3 & co);

private:

	Screen * currentScreen;
	bool isQuit, isStart;

	int pauseTime, clickChannel, backChannel, revChannel1, revChannel2,
		vehicleIndex;
	string titleMusic = "music/Faded_intro.wav";

	vec3 textColour;
	GLFWwindow *window;
	XboxController *controller;
	vector <XboxController> controllers;
	Audio *audio;

	ScreenOverlay background;

	Timer timer; 
	vector<Screen> screens;

	void pressStart(Audio *audio);
	void pressQuit();
};
