#include "..\headers\Game\Title\MultiplayerScreen.h"



MultiplayerScreen::MultiplayerScreen() :Screen()
{
	maxIndex = 3;
	screenIndex = 2;
	kerning = 20;
};

MultiplayerScreen::MultiplayerScreen(GLFWwindow *w, XboxController *x, Audio *a, vec3 &c) : Screen(w, x, a, c) {
	maxIndex = 3;
	screenIndex = 2;
	kerning = 20;

}

MultiplayerScreen::~MultiplayerScreen()
{
}

void MultiplayerScreen::Run()
{
	switch (KeyCallback(window, controller, audio)) {	//check key callback 
		//toggle
	case 2:
		toggleMenuIndex(1, initIndex, maxIndex);
		break;
		//toggle
	case 3:
		toggleMenuIndex(-1, initIndex, maxIndex);
		break;
		//press "select"
	case 4:
		audio->PlaySfx("armourAndSelect", MIX_MAX_VOLUME, select);
		//set flag to false
		isVisible = false;
		//get arena index (location of button in menuButtons vector -2)
		menuButtons[menuIndex].SetColour(pressColour);	//indicate choice
		menuButtons[menuIndex].SetMixFlag(1);
		break;
	case 5:
		backPress = true;
		isVisible = false;
		audio->PlaySfx("back", MIX_MAX_VOLUME, select);
		break;
	default:
		return;
	}
}


void MultiplayerScreen::Initialize()
{
	int numButtons = 5;
	//vector of buttons
	for (int i = 0; i < numButtons; i++) {
		menuButtons.emplace_back();
	}
	for (int i = 0; i < 4; i++) {
		menuButtons[i].InitializeGameText(to_string(i+1), vec3(-0.4 + (2 * i*0.1), -0.1, 0), textColour, kerning);
		menuButtons[i].SetScale(vec3(3.f));

	}
	//screen overlays for number of players
	menuButtons[4].InitializeGameText("How many players?", vec3(-0.6, 0.4, 0), textColour, kerning + 10);
	menuButtons[4].SetScale(vec3(2.f));

	//initialize which button cursor will be on upon starting
	menuButtons[0].SetColour(selectColour);	//is set to green initially
	menuButtons[0].SetMixFlag(1); //init

	for (int i = 0; i < numButtons; i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

}

int MultiplayerScreen::returnIndex() {
	return menuIndex + 1;
}