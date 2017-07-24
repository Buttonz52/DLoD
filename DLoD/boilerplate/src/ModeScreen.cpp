#include "..\headers\Game\Title\ModeScreen.h"

ModeScreen::ModeScreen() :Screen()
{
	maxIndex = 1;
	screenIndex = 3;
	kerning = 30;
};

ModeScreen::ModeScreen(GLFWwindow *w, XboxController *x, Audio *a) : Screen(w, x, a) {
	maxIndex = 1;
	screenIndex = 3;
	kerning = 30;
}
ModeScreen::~ModeScreen()
{
}

void ModeScreen::Run() {
		switch (KeyCallback(window, controller, audio)) {	//check key callback 
		case 2:
			toggleMenuIndex(1, initIndex, maxIndex);
			break;
		case 3:
			toggleMenuIndex(-1, initIndex, maxIndex);
			break;
			//press "select"
		case 4:
			isVisible = false;
			audio->PlaySfx("armourAndSelect", MIX_MAX_VOLUME,select);
			break;
			//press "back"
		case 5:
			isVisible = false;
			backPress = true;
			audio->PlaySfx("back", MIX_MAX_VOLUME, select);
			break;
		}
}

void ModeScreen::Initialize() {
	//vector of buttons
	int numButtons = 3;
	for (int i = 0; i < numButtons; i++) {
		menuButtons.emplace_back();
	}

	menuButtons[0].InitializeGameText("Timed", vec3(-0.6, 0, 0), vec3(0), kerning);
	menuButtons[1].InitializeGameText("Sudden Death", vec3(0.0, 0, 0), vec3(0), kerning);
	menuButtons[2].InitializeGameText("Choose mode", vec3(-0.4, 0.4, 0), vec3(0), kerning);
	
	for (int i =0; i <numButtons; i++)
		menuButtons[i].SetScale(vec3(2.f));

	//initialize which button cursor will be on upon starting
	menuButtons[0].SetColour(selectColour);	//is set to green initially
	menuButtons[0].SetMixFlag(1); //init
	prevColour = vec3(0, 0, 0);	//colour for first button

	for (int i = 0; i < numButtons; i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

