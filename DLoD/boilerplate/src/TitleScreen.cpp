#include "..\headers\Game\TitleScreen.h"

//TitleScreen cpp file.  Can definitely change around.



TitleScreen::TitleScreen()
{
	menuIndex;
//	this->audio = audio;
	buttonWidth = 0.2;
	buttonHeight = 0.1;
	padding = 0.05;
	isStart = false;
	isQuit = false;
	isRules = false;
	numMenuButtons = 3;
	prevCol = vec3(0);
	click = Mix_LoadWAV("sfx/bubblePop.wav");
	press = Mix_LoadWAV("sfx/startNoise.wav");
}


TitleScreen::~TitleScreen()
{
	Destroy();
}
//toggle startt boolean
void TitleScreen::pressStart(Audio *audio) {
	isStart = true;
	audio->PlaySfx(press);
}
//quit game
void TitleScreen::pressQuit() {
	isQuit = true;
}
//read rules (not implemented yet)
void TitleScreen::pressRules()
{
	isRules = !isRules;
}

//returns isStart
bool TitleScreen::isStartPressed() {
	return isStart;
}
//resturns isQuit
bool TitleScreen::isQuitPressed() {
	return isQuit;
}

//returns isRules flag
bool TitleScreen::isRulesPressed()
{
	return isRules;
}

//change buttons
void TitleScreen::toggleMenuIndex(const int &s, Audio *audio) {
	menuButtons[menuIndex].setColour(prevCol);	//button back to normal colour
	menuButtons[menuIndex].setMixColour(0);	//no longer mix colour
	menuIndex+=s;
	//top to bottom
	if (menuIndex < 0) {
		menuIndex = menuButtons.size() - 1;
	}
	//bottom to top
	if (menuIndex >= menuButtons.size()) {
		menuIndex = 0;
	}

	//set the previous colour
	prevCol = menuButtons[menuIndex].getColour();
	//new colour (green for the moment); used to mix with texture for current button (we can change this)
	menuButtons[menuIndex].setColour(vec3(0, 1, 0));
	//sex mixColour flag
	menuButtons[menuIndex].setMixColour(1);
	audio->PlaySfx(click);
	
}

void TitleScreen::Initialize() {

	//generic button
	ScreenOverlay button;
	//if (!audio.Init()) {
	//	cout << "Failed to init audio." << endl;
	//}
	//vector of buttons
	menuButtons.push_back(button);
	menuButtons.push_back(button);
	menuButtons.push_back(button);

	//set colours of buttons for now
	menuButtons[0].setColour(vec3(0, 1, 0));	//is set to green initially
	menuButtons[1].setColour(vec3(1, 0, 0));
	menuButtons[2].setColour(vec3(0, 1, 1));

	//Placeholder textures for better things.
	if (!menuButtons[0].initTexture("textures/START.png", GL_TEXTURE_2D)) {
		cout << "Failed to init start texture." << endl;
	}
	if (!menuButtons[1].initTexture("textures/RULES.png", GL_TEXTURE_2D)) {
		cout << "Failed to init rules texture." << endl;
	}
	if (!menuButtons[2].initTexture("textures/EXIT.png", GL_TEXTURE_2D)) {
		cout << "Failed to init exit texture." << endl;
	}

	//initialize which button cursor will be on upon starting
	menuButtons[0].setMixColour(1); //init
	prevCol = vec3(1, 1, 0);	//colour for first button

//	if (!background.initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
//		cout << "Failed to init texture." << endl;
//	}

	//generate vertices/scale
	cout << "Render generating vertices" << endl;
	background.GenerateSquareVertices(1, 1, vec3(0,0,1));	//invalid_enum occurs here
	cout << "vertices generated" << endl;

	//gererate for buttons
	for (int i = 0; i < menuButtons.size(); i++) {
		menuButtons[i].GenerateSquareVertices(buttonWidth, buttonHeight, menuButtons[i].getColour());
	}

	//set positions of buttons
	menuButtons[0].setPosition(vec3(0, 0, 0));
	menuButtons[1].setPosition(vec3(0, -(buttonHeight*2+ padding), 0));
	menuButtons[2].setPosition(vec3(0, -(buttonHeight*4 + padding*2), 0));

	//initialize shaders
	cout << "Init shaders" << endl;
	background.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	cout << "Inited shaders" << endl;

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

}

//render all components
void TitleScreen::Render() {

	//cout << "Render background" << endl;
	background.Render(GL_TRIANGLE_STRIP);	//render "loading screen"
	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].Render(GL_TRIANGLE_STRIP);	//render widget; this is just for testing at the moment

}

// handles keyboard input events for title screen
//TODO: IMPLEMENT FOR CONTROLLER

void TitleScreen::KeyCallback(GLFWwindow* window, XboxController *ctrller, Audio *audio)
{
	//Please test to 
	if (ctrller->Connected()) {
		//down
		if (ctrller->GetButtonPressed(XBtns.DPad_Up)) {
			toggleMenuIndex(-1, audio);
		}
		else if (ctrller->GetButtonPressed(XBtns.DPad_Down)) {
			toggleMenuIndex(1, audio);
		}
		//Don't know if this is correct..
		else if (ctrller->GetButtonPressed(XBtns.A)) {
			switch (menuIndex)
			{
			case 0:
				pressStart(audio);	//start game
				break;
			case 1:
				pressRules();	//read rules (not implemented yet)
				break;
			case 2:
				pressQuit();	//quit game
				break;
			}
		}

	}
	else {
		int state;
		// up menu
		state = glfwGetKey(window, GLFW_KEY_UP);
		if (state == GLFW_PRESS)
			toggleMenuIndex(-1, audio);

		//down menu
		state = glfwGetKey(window, GLFW_KEY_DOWN);
		if (state == GLFW_PRESS)
			toggleMenuIndex(1, audio);

		//select
		state = glfwGetKey(window, GLFW_KEY_ENTER);
		if (state == GLFW_PRESS)
			switch (menuIndex)
			{
			case 0:
				pressStart(audio);	//start game
				break;
			case 1:
				pressRules();	//read rules (not implemented yet)
				break;
			case 2:
				pressQuit();	//quit game
				break;
			}
	}
}

bool TitleScreen::Display(GLFWwindow *window, XboxController *controller, Audio *audio) {

	//initialize title screen
	Initialize();

	//loop until something happens
	while (!isStartPressed()) {
		// clear screen to a dark grey colour;
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render();		//render titlescreen
		glfwSwapBuffers(window);	//need this to output to screen
		KeyCallback(window, controller, audio);	//check key callback

													//if quit selected, or other input to close program selected, close program
		if (isQuitPressed() || glfwWindowShouldClose(window)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
			Destroy();
			glfwDestroyWindow(window);
			glfwTerminate();
			return 0;
		}
		Sleep(100);		//slow down input so not crazy fast
		glfwPollEvents();
	}
	Destroy();	//destroy title page
	return 1;
	//Initialize "Loading screen"
}
//clean up
void TitleScreen::Destroy() {
	background.Destroy();
	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].Destroy();
}