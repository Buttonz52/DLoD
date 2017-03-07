#pragma once
#include "Utility.h"
#include "../GameEngine/Mesh.h"
#include "ScreenOverlay.h"
#include "../Controller/XboxController.h"
#include "Audio.h"

class TitleScreen
{
public:
	TitleScreen();
	~TitleScreen();

	bool isStartPressed();
	bool isQuitPressed();
	bool isRulesPressed();

	void InitializeTitleScreen();
	void InitializeChooseScreen();
	void Render();
	bool DisplayTitle(GLFWwindow *, XboxController *controller, Audio *audio, int &skyboxIndex, int &arenaIndex);
	void Destroy();

	int KeyCallback(GLFWwindow* window, XboxController *ctrller, Audio *audio);

private:
	bool isQuit, isStart, isRules, isLoadScreen, isChooseArena;
	int menuIndex;
	int numMenuButtons;
	float buttonWidth;
	float buttonHeight;
	float padding;
	vec3 selectColour;
	vec3 pressColour;
	ScreenOverlay background;
	vector<ScreenOverlay> menuButtons;

	vec3 prevCol;
	void pressStart(Audio *audio);
	void pressQuit();
	void pressRules();
	void toggleMenuIndex(const int &s, Audio *audio);
	XboxController controller;
	Mix_Chunk *click;
	Mix_Chunk *press;
	Mix_Chunk *back;
	//string backgroundTexture; //might not need
	//string startTexture;	//might not need
	//string rulesTexture;	//might not need
	//string quitTexture;	//might not need 
};



