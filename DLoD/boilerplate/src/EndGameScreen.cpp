#include "..\headers\Game\EndGameScreen.h"



EndGameScreen::EndGameScreen() :Screen()
{
	isRestart = false;
	isQuit = false;
	screenIndex = 7;
}

EndGameScreen::EndGameScreen(GLFWwindow * w, XboxController * x, Audio * a, vec3 & c, const vector<string> &names, const vector<vec3> &colours) : Screen(w,x,a, vec3())
{
	initIndex = 0;
	maxIndex = 1;
	screenIndex = 7;
	kerning = 30;
	isRestart = false;
	isQuit = false;
	selectColour = c;
	playerNames = names;
	playerColours = colours;
}

EndGameScreen::~EndGameScreen()
{
}

string EndGameScreen::formatRank(const string &player, const int &index) {
	std::stringstream fmt;
	fmt << index;
	switch (index) {
	case 1:
		fmt << "st: ";
		break;
	case 2:
		fmt << "nd: ";
		break;
	case 3:
		fmt << "rd: ";
		break;
	default:
		fmt << "th: ";
		break;
	}
	fmt << player;
	return fmt.str();
}

void EndGameScreen::Initialize()
{
	int numButtons = 13;
	for (int i = 0; i < numButtons; i++)
		menuButtons.emplace_back();
	menuButtons[0].InitializeGameText("RESTART", vec3(-0.29, -0.7, 0), textColour, kerning);
	menuButtons[0].SetScale(vec3(2));
	menuButtons[1].InitializeGameText("QUIT", vec3(-0.19, -0.9, 0), textColour, kerning);
	menuButtons[1].SetScale(vec3(2));

	menuButtons[0].SetColour(selectColour);
	menuButtons[0].SetMixFlag(1);

	vec3 size = vec3(0.8f);
	float y_offset = 0.4f;
	for (int i = 0; i < playerNames.size(); i++) {
		string str = formatRank(playerNames[i], i + 1);
		menuButtons[i+2].InitializeGameText(str, vec3(-0.5, y_offset-(i*0.12), 0), playerColours[i], kerning);
		menuButtons[i + 2].SetScale(size);
	}

	menuButtons[10].SetScale(vec3(4.f));
	menuButtons[10].InitializeGameText("END GAME", vec3(-0.57, 0.5, 0), textColour, kerning);

	//arena map background
	if (!menuButtons[12].InitTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
		cout << "Failed to init dlod texture" << endl;
	}

	menuButtons[11].GenerateSquareVertices(0.5, 0.5, vec3());
	//menuButtons[7].SetTransparency(0.5f);

	menuButtons[12].GenerateSquareVertices(1, 1, vec3(0.6, 0.5, 1));
	menuButtons[12].SetMixFlag(1);
	menuButtons[12].SetMixAmount(0.5);

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
	default:
		return;
	}
}
