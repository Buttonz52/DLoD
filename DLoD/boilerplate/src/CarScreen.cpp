#include "..\headers\Game\Title\CarScreen.h"



CarScreen::CarScreen() :Screen()
{
	maxIndex = 2;
	screenIndex = 4;
	controllerIndex = 0;
	numHumans = 1;
	kerning = 32;
};

CarScreen::CarScreen(GLFWwindow * w, vector<XboxController> &x, Audio * a, const int &numPlayers, vec3 &c) :Screen(w,NULL,a,c)
{
	maxIndex = 2;
	screenIndex = 4;
	controllerIndex = 0;
	numHumans = numPlayers;
	controllers = x;
	kerning = 32;
};

CarScreen::~CarScreen()
{
}

void CarScreen::Run()
{
	switch (KeyCallback(window, &controllers[controllerIndex], audio)) {	//check key callback 
	case 2:
		toggleMenuIndex(1, initIndex, maxIndex);
		break;
	case 3:
		toggleMenuIndex(-1, initIndex, maxIndex);
		break;
		//press "select"
	case 4: {
		audio->PlaySfx("armourAndSelect", MIX_MAX_VOLUME, select);
		//set flag to false
		menuButtons[menuIndex].SetColour(pressColour);	//indicate choice
		menuButtons[menuIndex].SetMixFlag(1);

		controllerIndex++;
		isVisible = false;

		std::stringstream fmt;
		int controllerIndexTextInt = controllerIndex + 1;
		if (controllerIndexTextInt <= numHumans) {
			fmt << "Player " << controllerIndexTextInt << ", choose your vehicle";
			menuButtons[3].UpdateGameText(fmt.str(), kerning);
		}
		menuButtons[menuIndex].SetColour(selectColour);	//indicate choice
	}
		break;
		//press "back"
	case 5:
		backPress = true;
		isVisible = false;
		audio->PlaySfx("back", MIX_MAX_VOLUME, select);
		break;
	default:
		return;
	}
}

void CarScreen::Initialize()
{
	int numButtons = 4;

	//vector of buttons
	for (int i = 0; i <numButtons; i++) {
		menuButtons.emplace_back();
	}
	//vehicle choice buttons
	if (!menuButtons[0].InitTexture("textures/carImgs/lightCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init light car texture." << endl;
	}
	if (!menuButtons[1].InitTexture("textures/carImgs/mediumCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init medium car texture." << endl;
	}
	if (!menuButtons[2].InitTexture("textures/carImgs/heavyCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init heavy car texture." << endl;
	}
	menuButtons[3].InitializeGameText("Player 1, choose your vehicle", vec3(-0.95, 0.4, 0), textColour, kerning);
	menuButtons[3].SetScale(vec3(2.f));

	menuButtons[0].SetPosition(vec3(-0.4, 0, 0));
	menuButtons[1].SetPosition(vec3(0, 0, 0));
	menuButtons[2].SetPosition(vec3(0.4, 0, 0));

	//gererate for buttons
	for (int i = 0; i < numButtons -1; i++)
		menuButtons[i].GenerateSquareVertices(0.2, 0.2, menuButtons[i].getColour());

	//initialize which button cursor will be on upon starting
	menuButtons[0].SetColour(selectColour);	//is set to green initially
	menuButtons[0].SetMixFlag(1); //init

	for (int i = 0; i < numButtons; i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

