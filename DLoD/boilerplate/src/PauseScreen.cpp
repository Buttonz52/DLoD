#include "Game\PauseScreen.h"



PauseScreen::PauseScreen() :Screen()
{
	isRestart = false;
	isQuit = false;
	screenIndex = 6;
}

PauseScreen::PauseScreen(GLFWwindow *w, XboxController *x, Audio *a, vec3 &c) : Screen(w, x, a, vec3()) {
	initIndex = 0;
	maxIndex = 2;
	screenIndex = 6;
	kerning = 30;
	selectColour = c;
	isRestart = false;
	isQuit = false;
}

int PauseScreen::KeyCallback(GLFWwindow *w, XboxController *c, Audio * a) {
	int callback;
	if ((callback = Screen::KeyCallback(w, c, a)) != -1) {
		return callback;
	}
	//controller input
	if (c->Connected()) {
		if (c->GetButtonPressed(XBtns.StartBtn))
			return 6;
		else
			return -1;
	}
	//keyboard input
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return 6;
	return -1;
}
PauseScreen::~PauseScreen()
{
}

void PauseScreen::Initialize()
{
	int numButtons = 5;
	for (int i = 0; i < numButtons; i++)
		menuButtons.emplace_back();

	menuButtons[0].InitializeGameText("RESUME", vec3(-0.27, 0.0, 0), textColour, kerning);
	menuButtons[0].SetScale(vec3(2));
	menuButtons[1].InitializeGameText("RESTART", vec3(-0.3, -0.2, 0), textColour, kerning);
	menuButtons[1].SetScale(vec3(2));
	menuButtons[2].InitializeGameText("QUIT", vec3(-0.2, -0.4, 0), textColour, kerning);
	menuButtons[2].SetScale(vec3(2));

	menuButtons[3].SetScale(vec3(4.f));
	menuButtons[3].InitializeGameText("PAUSE", vec3(-0.4, 0.5, 0), textColour, kerning);

	//arena map background
	if (!menuButtons[4].InitTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
		//	cout << "Failed to init arena map." << endl;
	}

	menuButtons[4].GenerateSquareVertices(1, 1, vec3(0.6, 0.5, 1));
	menuButtons[4].SetMixFlag(1);
	menuButtons[4].SetMixAmount(0.5);
	//menuButtons[3].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");


	menuButtons[0].SetColour(selectColour);
	menuButtons[0].SetMixFlag(1);

	for (int i = 0; i < numButtons; i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

}

void PauseScreen::Run()
{
	int key = KeyCallback(window, controller, audio);
	switch (key) {	//check key callback 
	case 0:
		toggleMenuIndex(-1, initIndex, maxIndex);
		break;
	case 1:
		toggleMenuIndex(1, initIndex, maxIndex);
		break;
	case 4:
		audio->PlaySfx("armourAndSelect", MIX_MAX_VOLUME, select);
		menuButtons[menuIndex].SetColour(pressColour);	//indicate choice
		menuButtons[menuIndex].SetMixFlag(1);
		switch (menuIndex)
		{
			case 0:
				backPress = true;
				isVisible = false;
				break;
			case 1:
				isRestart = true;
				isVisible = false;
				break;
			case 2:
				isQuit = true;
				isVisible = false;
				break;
		}
		break;

	//back or escape/start
	case 5:
	//case 6:
		backPress = true;
		isVisible = false;
		audio->PlaySfx("back", MIX_MAX_VOLUME, select);
		break;
	default:
		return;
	}
}
