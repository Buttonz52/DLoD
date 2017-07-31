#include "..\headers\Game\EndGameScreen.h"



EndGameScreen::EndGameScreen() :Screen()
{
	isRestart = false;
	isQuit = false;
	screenIndex = 7;
}

EndGameScreen::EndGameScreen(GLFWwindow * w, XboxController * x, Audio * a, vec3 & c, const vector<string> &names, const vector<vec3> &colours) : Screen(w,x,a, c)
{
	initIndex = 0;
	maxIndex = 1;
	screenIndex = 7;
	kerning = 30;
	isRestart = false;
	isQuit = false;
	playerNames = names;
	playerColours = colours;
}

EndGameScreen::~EndGameScreen()
{
}

void EndGameScreen::Initialize()
{
	int numButtons = 9;
	for (int i = 0; i < numButtons; i++)
		menuButtons.emplace_back();
	menuButtons[0].InitializeGameText("RESTART", vec3(-0.29, -0.7, 0), textColour, kerning);
	menuButtons[0].SetScale(vec3(2));
	menuButtons[1].InitializeGameText("QUIT", vec3(-0.19, -0.9, 0), textColour, kerning);
	menuButtons[1].SetScale(vec3(2));

	menuButtons[0].SetColour(selectColour);
	menuButtons[0].SetMixFlag(1);

	std::stringstream fmt1;
	fmt1 << "1st place: " << playerNames[0];
	menuButtons[2].InitializeGameText(fmt1.str(), vec3(-0.35, 0.3, 0), playerColours[0], kerning);
	std::stringstream fmt2;
	fmt2 << "2nd place: " << playerNames[1];
	menuButtons[3].InitializeGameText(fmt2.str(), vec3(-0.35, 0.1, 0), playerColours[1], kerning);
	std::stringstream fmt3;
	fmt3 << "3rd place: " << playerNames[2];
	menuButtons[4].InitializeGameText(fmt3.str(), vec3(-0.35, -0.1, 0), playerColours[2], kerning);
	std::stringstream fmt4;
	fmt4 << "4th place: " << playerNames[3];
	menuButtons[5].InitializeGameText(fmt4.str(), vec3(-0.35, -0.3, 0), playerColours[3], kerning);

	menuButtons[6].SetScale(vec3(4.f));
	menuButtons[6].InitializeGameText("END GAME", vec3(-0.57, 0.5, 0), textColour, kerning);

	//arena map background
	if (!menuButtons[8].InitTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
		cout << "Failed to init dlod texture" << endl;
	}

	menuButtons[7].GenerateSquareVertices(0.5, 0.5, vec3());
	//menuButtons[7].SetTransparency(0.5f);

	menuButtons[8].GenerateSquareVertices(1, 1, vec3(0.6, 0.5, 1));
	menuButtons[8].SetMixFlag(1);
	menuButtons[8].SetMixAmount(0.5);

	for (int i = 0; i < numButtons; i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

void EndGameScreen::Run()
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
			isRestart = true;
			isVisible = false;
			break;
		case 1:
			isQuit = true;
			isVisible = false;
			break;
		}
		break;
	}
}
