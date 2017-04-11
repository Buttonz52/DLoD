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

private:
	bool isQuit, isStart, 
		isRules, isLoadScreen, 
		isChooseArena, isChooseSkybox, 
		isMultiplayerScreen, isCarScreen,
		isArenaSkyboxScreen;

	int menuIndex,
		multiplayerInitIndex,
		arenaButtonInitIndex,
		skyboxButtonInitIndex,
		carButtonInitIndex,
		controllerIndex;

	int numMenuButtons;
	int initIndex, maxIndex;
	int pauseTime;
	int clickChannel, backChannel, revChannel1, revChannel2;
	string titleMusic = "music/Faded_intro.wav";

	vec3 selectColour, pressColour, prevColour;

	ScreenOverlay background;
	vector<ScreenOverlay> menuButtons;

	Timer timer; 
	
	Mix_Chunk *click, *press, *back, *rev1, *rev2, *rev3, *rev4;

	void pressStart(Audio *audio);
	void readRules(GLFWwindow * window, XboxController * ctrller, Audio * audio, int &skyboxIndex, int &arenaIndex, vector<int> *humanVehicleChoice, int &numPlayers);
	void DisplayVideo(Audio *audio);
	void CheckTimeout(Audio *audio);
	void pressQuit();
	void toggleMenuIndex(const int &s, Audio *audio, const int &initIndex, const int &maxIndex);
};
