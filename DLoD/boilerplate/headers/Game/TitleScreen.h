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

	void Initialize();
	void Render();
	bool Display(GLFWwindow *, XboxController *controller, Audio *audio);
	void Destroy();

	void KeyCallback(GLFWwindow* window, XboxController *ctrller, Audio *audio);

private:
	bool isQuit, isStart, isRules;
	int menuIndex;
	int numMenuButtons;
	float buttonWidth;
	float buttonHeight;
	float padding;

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
	//string backgroundTexture; //might not need
	//string startTexture;	//might not need
	//string rulesTexture;	//might not need
	//string quitTexture;	//might not need 
};



