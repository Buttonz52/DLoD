#pragma once
#include "Utility.h"
#include "GameEngine/Mesh.h"
#include "ScreenOverlay.h"
#include "Controller/XboxController.h"
#include "Audio.h"
#include "Game\timer.h"

class TitleScreen
{
public:
	TitleScreen();
	~TitleScreen();

	bool isStartPressed();
	bool isRulesPressed();
	bool isQuitPressed();

	void InitializeTitleScreen();
	void InitializeMultiplayerScreen();
	void InitializeCarScreen();
	void InitializeChooseScreen();

	void Render();
	bool DisplayTitle(GLFWwindow *, XboxController *controller, Audio *audio, int &skyboxIndex, int &arenaIndex, vector <int> *humanVehicleChoice, int &numPlayers);

	void newMenuIndex(int &newMenuIndex, const int &newInitIndex, int &initIndex, int &maxIndex, const int &addIndex);
	int KeyCallback(GLFWwindow* window, XboxController *ctrller, Audio *audio);
	void Destroy();

	//void titleScreenLoop(int key);

private:
	bool isQuit, isStart, 
		isRules, isLoadScreen, 
		isChooseArena, isChooseSkybox, 
		isMultiplayerScreen, isCarScreen;

	int menuIndex,
		multiplayerInitIndex,
		arenaButtonInitIndex,
		skyboxButtonInitIndex,
		carButtonInitIndex,
		controllerIndex;

	int numMenuButtons;
	int initIndex, maxIndex;
	string titleMusic = "music/Faded_intro.wav";

	vec3 selectColour, pressColour, prevColour;

	ScreenOverlay background;
	vector<ScreenOverlay> menuButtons;

	Mix_Chunk *click, *press, *back;

	void pressStart(Audio *audio);
	void readRules(GLFWwindow * window, XboxController * ctrller, Audio * audio, int &skyboxIndex, int &arenaIndex, vector<int> *humanVehicleChoice, int &numPlayers);
	void pressQuit();
	void pressRules();
	void toggleMenuIndex(const int &s, Audio *audio, const int &initIndex, const int &maxIndex);
};
