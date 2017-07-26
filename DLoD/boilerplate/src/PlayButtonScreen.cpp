#include "..\headers\Game\Title\PlayButtonScreen.h"



PlayButtonScreen::PlayButtonScreen() :Screen()
{
	screenIndex = 5;
	kerning = 20;
};

PlayButtonScreen::PlayButtonScreen(GLFWwindow *w, XboxController *x, Audio *a, vec3 &c) :Screen(w,x,a, c)
{
	screenIndex = 5;
	kerning = 20;
};

PlayButtonScreen::~PlayButtonScreen()
{
}

void PlayButtonScreen::Run()
{
	switch (KeyCallback(window, controller, audio)) {	//check key callback 
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
	}
}

void PlayButtonScreen::Initialize()
{
	menuButtons.emplace_back();
	menuButtons[0].InitializeGameText("PLAY", vec3(-0.4,-0.2,0), textColour, kerning + 10);
	menuButtons[0].SetScale(vec3(5.f));
	menuButtons[0].SetColour(selectColour);
	menuButtons[0].SetMixFlag(1);
	menuButtons[0].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

}

int PlayButtonScreen::returnIndex() {
	return backPress;
}