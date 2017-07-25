#include "..\headers\Game\Title\MainScreen.h"



MainScreen::MainScreen() :Screen()
{
	initIndex = 0;
	maxIndex = 2;
	quit = false;
	rulesScreen = false;
	screenIndex = 0;
	kerning = 20;
};

MainScreen::MainScreen(GLFWwindow *w, XboxController *x, Audio *a) : Screen(w,x,a){
	initIndex = 0;
	maxIndex = 2;
	quit = false;
	rulesScreen = false;
	screenIndex = 0;
	kerning = 20;

}

MainScreen::~MainScreen()
{
}

void MainScreen::Run()
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
		switch (menuIndex)
		{
		case 0:
			audio->PlaySfx("armourAndSelect", MIX_MAX_VOLUME, select);
			//destroy menu buttons
			for (int i = 0; i < menuButtons.size(); i++)
				menuButtons[i].Destroy();

			isVisible = false;
			break;
		case 1:
			audio->PlaySfx("toggle", MIX_MAX_VOLUME, menu);
			isVisible = false;
			rulesScreen = true;
			break;
		case 2:
			isVisible = false;
			quit = true;
			break;
		}
		break;
	}
}

bool MainScreen::checkQuit() {
	return quit;
}

bool MainScreen::checkRules() {
	return rulesScreen;
}
void MainScreen::Initialize()
{
	//vector of buttons
	int numButtons = 5;
	for (int i = 0; i < numButtons; i++)
		menuButtons.emplace_back();

	//set colours of buttons for now
	menuButtons[0].SetColour(selectColour);	//is set to green initially

	float menuXPosition = -0.6;
	float menuYPosition = -0.2;
	menuButtons[0].InitializeGameText("START", vec3(menuXPosition, menuYPosition, 0), selectColour, kerning);
	menuButtons[1].InitializeGameText("CONTROLS", vec3(menuXPosition, menuYPosition - 0.1, 0), vec3(0.2, 0.2, 0), kerning);
	menuButtons[2].InitializeGameText("QUIT", vec3(menuXPosition, menuYPosition - 0.2, 0), vec3(0.2, 0.2, 0), kerning);

	menuButtons[3].InitializeGameText("DERBY LEAGUE", vec3(-0.6, 0.6, 0), vec3(0, 0, 0), kerning);
	menuButtons[4].InitializeGameText("OF DESTRUCTION", vec3(-0.7, 0.3, 0), vec3(0, 0, 0), kerning);

	menuButtons[3].SetScale(vec3(2));
	menuButtons[4].SetScale(vec3(2));

	/*menuButtons[5].GenerateSquareVertices(0.25, 0.25, vec3(1));
	menuButtons[5].InitTexture("textures/DLoDLogo.png", GL_TEXTURE_2D);
	menuButtons[5].SetPosition(vec3(0.4, -0.1, 0));
*/
	//initialize which button cursor will be on upon starting
	menuButtons[0].SetMixFlag(1); //init
	prevColour = vec3(0.2, 0.2, 0);	//colour for first button

	//generate vertices/scale
	float buttonWidth = 0.2;
	float buttonHeight = 0.1;
	float padding = 0.05;														//gererate for buttons

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

}
