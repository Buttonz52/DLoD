#include "Game\TitleScreen.h"

//TitleScreen cpp file.  Can definitely change around.



TitleScreen::TitleScreen()
{
	isStart = false;
	isRules = false;
	isQuit = false;
	isChooseArena = false;
	isChooseSkybox = false;
	isLoadScreen = false;
	isCarScreen = false;

	menuIndex = 0;
	multiplayerInitIndex = 1;
	arenaButtonInitIndex = 2;
	skyboxButtonInitIndex = 5;
	carButtonInitIndex = 1;
	controllerIndex = 0;

	numMenuButtons = 3;

	selectColour = vec3(0,1,0);
	pressColour = vec3(1, 0, 1);
	prevColour = vec3(0);

	click = Mix_LoadWAV("sfx/bubblePop.wav");
	press = Mix_LoadWAV("sfx/startNoise.wav");
	back = Mix_LoadWAV("sfx/backSfx.wav");
}


TitleScreen::~TitleScreen()
{
	Destroy();
}

//toggle startt boolean
void TitleScreen::pressStart(Audio *audio) {
	isStart = true;
	audio->PlaySfx(press, MIX_MAX_VOLUME,1);
}

//read rules (not implemented yet)
void TitleScreen::readRules(GLFWwindow *window, XboxController *ctrller, Audio *audio, int &skyboxIndex, int &arenaIndex, vector<int> *humanVehicleChoice, int &numPlayers)
{
	ScreenOverlay rulesScreen;
	ScreenOverlay rulesBackText;

	rulesBackText.InitializeGameText("Press B to go back", vec3(-0.15,0.1,0), vec3(0,0,1), 30);
	rulesBackText.setScale(vec3(0.5));
	if (!rulesScreen.initTexture("textures/rulesScreen.png", GL_TEXTURE_2D)) {
		cout << "Failed to init rules screen." << endl;
	}
	rulesScreen.GenerateSquareVertices(1, 1, vec3(0,0,0));

	rulesScreen.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	rulesBackText.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	int t = 0;
	while (!glfwWindowShouldClose(window)) {

		int key = KeyCallback(window, ctrller, audio);
		if (key ==5) {
			rulesScreen.Destroy();
			rulesBackText.Destroy();
			return;
		}rulesBackText.setTransparency(float(t % 20)*0.1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		background.Render(GL_TRIANGLE_STRIP, background.getColour());	//render "loading screen"
		rulesScreen.Render(GL_TRIANGLE_STRIP, rulesScreen.getColour());	//render text, as it uses triangles, not triangle strip
		rulesBackText.Render(GL_TRIANGLES, rulesBackText.getColour());
		glfwSwapBuffers(window);	//need this to output to screen
		Sleep(100);		//slow down input so not crazy fast
		glfwPollEvents();
		t++;
	}
}

//quit game
void TitleScreen::pressQuit() {
	isQuit = true;
}


//returns isStart
bool TitleScreen::isStartPressed() {
	return isStart;
}


//resturns isQuit
bool TitleScreen::isQuitPressed() {
	return isQuit;
}

//change buttons
void TitleScreen::toggleMenuIndex(const int &s, Audio *audio, const int &initIndex, const int &maxIndex) {
	menuButtons[menuIndex].setColour(prevColour);	//button back to normal colour
	menuButtons[menuIndex].setMixFlag(0);	//no longer mix colour
	menuIndex += s;

	//keep index in bounds
	if (menuIndex > maxIndex)
		menuIndex = initIndex;
	if (menuIndex < initIndex)
		menuIndex = maxIndex;

	//set the previous colour
	prevColour = menuButtons[menuIndex].getColour();
	//new colour (green for the moment); used to mix with texture for current button (we can change this)
	menuButtons[menuIndex].setColour(selectColour);
	//sex mixColour flag
	menuButtons[menuIndex].setMixFlag(1);
	audio->PlaySfx(click, MIX_MAX_VOLUME,1);
}

//Initializes main screen
void TitleScreen::InitializeTitleScreen() {

	//clear vector of menu buttons
	menuButtons.clear();

	//generic button with transparent background
	ScreenOverlay button;

	//vector of buttons
	for (int i = 0; i < 5; i++)
		menuButtons.push_back(button);

	//set colours of buttons for now
	menuButtons[0].setColour(selectColour);	//is set to green initially

											//Initialize button textures
	if (!menuButtons[0].initTexture("textures/START.png", GL_TEXTURE_2D)) {
		cout << "Failed to init start texture." << endl;
	}
	if (!menuButtons[1].initTexture("textures/RULES.png", GL_TEXTURE_2D)) {
		cout << "Failed to init rules texture." << endl;
	}
	if (!menuButtons[2].initTexture("textures/EXIT.png", GL_TEXTURE_2D)) {
		cout << "Failed to init exit texture." << endl;
	}

	menuButtons[3].InitializeGameText("DERBY LEAGUE", vec3(-0.6, 0.6, 0), vec3(0, 0, 0), 20);
	menuButtons[4].InitializeGameText("OF DESTRUCTION", vec3(-0.7, 0.3, 0), vec3(0, 0, 0), 20);

	menuButtons[3].setScale(vec3(2));
	menuButtons[4].setScale(vec3(2));

	//initialize which button cursor will be on upon starting
	menuButtons[0].setMixFlag(1); //init
	prevColour = vec3(1, 1, 0);	//colour for first button

	//generate vertices/scale
	background.GenerateSquareVertices(1, 1, vec3(1, 0, 0));	//invalid_enum occurs here

	float buttonWidth = 0.2;
	float buttonHeight = 0.1;
	float padding = 0.05;														//gererate for buttons
	for (int i = 0; i < menuButtons.size() - 2; i++) {
		menuButtons[i].GenerateSquareVertices(buttonWidth, buttonHeight, menuButtons[i].getColour());
	}

	//set positions of buttons
	menuButtons[0].setPosition(vec3(0, 0, 0));
	menuButtons[1].setPosition(vec3(0, -(buttonHeight * 2 + padding), 0));
	menuButtons[2].setPosition(vec3(0, -(buttonHeight * 4 + padding * 2), 0));

	//initialize shaders for background and buttons
	background.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

void TitleScreen::InitializeMultiplayerScreen() {
	//clear vector of menu buttons
	menuButtons.clear();

	//generic button
	ScreenOverlay button;

	//vector of buttons
	for (int i = 0; i <5; i++) {
		menuButtons.push_back(button);
	}

	//screen overlays for number of players
	menuButtons[0].InitializeGameText("How many players?", vec3(-0.6, 0.4, 0), vec3(0), 30);
	menuButtons[0].setScale(vec3(2.f));
	for (int i = 1; i < 5; i++) {
		menuButtons[i].InitializeGameText(to_string(i), vec3(-0.6 + (2 * i*0.1), -0.2, 0), vec3(0, 0, 0), 20);
		menuButtons[i].setScale(vec3(3.f));

	}
	//initialize which button cursor will be on upon starting
	menuButtons[multiplayerInitIndex].setColour(selectColour);	//is set to green initially
	menuButtons[multiplayerInitIndex].setMixFlag(1); //init
	prevColour = vec3(0, 0, 0);	//colour for first button

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

void TitleScreen::InitializeCarScreen() {
	//clear vector of menu buttons
	menuButtons.clear();

	//generic button
	ScreenOverlay button;

	//vector of buttons
	for (int i = 0; i <4; i++) {
		menuButtons.push_back(button);
	}
	menuButtons[0].InitializeGameText("Player 1, choose your vehicle:", vec3(-0.5, 0.3, 0), vec3(0), 30);

	//vehicle choice buttons
	if (!menuButtons[carButtonInitIndex].initTexture("textures/carImgs/lightCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init light car texture." << endl;
	}
	if (!menuButtons[carButtonInitIndex + 1].initTexture("textures/carImgs/mediumCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init medium car texture." << endl;
	}
	if (!menuButtons[carButtonInitIndex + 2].initTexture("textures/carImgs/heavyCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init heavy car texture." << endl;
	}

	menuButtons[carButtonInitIndex].setPosition(vec3(-0.4, 0, 0));
	menuButtons[carButtonInitIndex + 1].setPosition(vec3(0, 0, 0));
	menuButtons[carButtonInitIndex + 2].setPosition(vec3(0.4, 0, 0));

	//gererate for buttons
	for (int i = carButtonInitIndex; i < menuButtons.size(); i++)
		menuButtons[i].GenerateSquareVertices(0.2, 0.2, menuButtons[i].getColour());

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}
//initializes choose arena screen
void TitleScreen::InitializeChooseScreen() {

	//clear vector of menu buttons
	menuButtons.clear();

	//generic button
	ScreenOverlay button;

	//vector of buttons
	for (int i = 0; i <9; i++) {
		menuButtons.push_back(button);
	}

	//screen overlays for arena and location
	menuButtons[0].InitializeGameText("Choose your arena", vec3(-0.4, 0.7, 0), vec3(0), 20);
	menuButtons[1].InitializeGameText("Choose your location", vec3(-0.5, 0.0, 0), vec3(0), 20);

	//init transparent arena textures
	if (!menuButtons[arenaButtonInitIndex].initTexture("textures/arenaImgs/bowl.png", GL_TEXTURE_2D)) {
		cout << "Failed to init bowl texture." << endl;
	}
	if (!menuButtons[arenaButtonInitIndex + 1].initTexture("textures/arenaImgs/devil.png", GL_TEXTURE_2D)) {
		cout << "Failed to init devil texture." << endl;
	}
	if (!menuButtons[arenaButtonInitIndex + 2].initTexture("textures/arenaImgs/wrap.png", GL_TEXTURE_2D)) {
		cout << "Failed to init wrap texture." << endl;
	}

	//initialize which button cursor will be on upon starting
	menuButtons[arenaButtonInitIndex].setColour(selectColour);	//is set to green initially
	menuButtons[arenaButtonInitIndex].setMixFlag(1); //init
	prevColour = vec3(1, 1, 0);	//colour for first button

								//textures for skybox choice buttons
	if (!menuButtons[skyboxButtonInitIndex].initTexture("textures/ame_ash/ashcanyon_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex + 1].initTexture("textures/nec_hell/hell_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex + 2].initTexture("textures/mp_deviltooth/devils-tooth_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex + 3].initTexture("textures/mp_emerald/emerald-summit_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}

	//gererate for buttons
	for (int i = arenaButtonInitIndex; i < menuButtons.size(); i++) {
		menuButtons[i].GenerateSquareVertices(0.2, 0.2, menuButtons[i].getColour());
	}

	float arena_y = 0.4, skybox_y = -0.3;
	//set positions of buttons
	menuButtons[arenaButtonInitIndex].setPosition(vec3(-0.4, arena_y, 0));
	menuButtons[arenaButtonInitIndex + 1].setPosition(vec3(0.0, arena_y, 0));
	menuButtons[arenaButtonInitIndex + 2].setPosition(vec3(0.4, arena_y, 0));

	menuButtons[skyboxButtonInitIndex].setPosition(vec3(-0.6, skybox_y, 0));
	menuButtons[skyboxButtonInitIndex + 1].setPosition(vec3(-0.2, skybox_y, 0));
	menuButtons[skyboxButtonInitIndex + 2].setPosition(vec3(0.2, skybox_y, 0));
	menuButtons[skyboxButtonInitIndex + 3].setPosition(vec3(0.6, skybox_y, 0));

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}
//render all components
void TitleScreen::Render() {
	background.Render(GL_TRIANGLE_STRIP, background.getColour());	//render "loading screen"
	for (int i = 0; i < menuButtons.size(); i++)
		if (!menuButtons[i].isFontTex) {
			menuButtons[i].Render(GL_TRIANGLE_STRIP, menuButtons[i].getColour());	//render regular button
		}
		else {
			menuButtons[i].Render(GL_TRIANGLES, menuButtons[i].getColour());	//render text, as it uses triangles, not triangle strip
		}
}

// handles keyboard input events for title screen
int TitleScreen::KeyCallback(GLFWwindow* window, XboxController *ctrller, Audio *audio)
{
	//controller
	if (ctrller->Connected()) {
		//down
		if (ctrller->GetButtonPressed(XBtns.DPad_Up)) {
			return 0;
		}
		else if (ctrller->GetButtonPressed(XBtns.DPad_Down)) {
			return 1;
		}
		else if (ctrller->GetButtonPressed(XBtns.DPad_Right)) {
			return 2;
		}
		else if (ctrller->GetButtonPressed(XBtns.DPad_Left)) {
			return 3;
		}
		else if (ctrller->GetButtonPressed(XBtns.A)) {
			return 4;
		}
		else if (ctrller->GetButtonPressed(XBtns.B)) {
			return 5;
		}
		else if (ctrller->GetButtonPressed(XBtns.X)) {
			return 6;
		}
		else {
			return -1;
		}
	}

	//keyboard input for player 1 only
	//else if (ctrller->GetIndex() == 0) {
		int state;
		// up menu
		state = glfwGetKey(window, GLFW_KEY_UP);
		if (state == GLFW_PRESS)
			return 0;

		//down menu
		state = glfwGetKey(window, GLFW_KEY_DOWN);
		if (state == GLFW_PRESS)
			return 1;

		//right
		state = glfwGetKey(window, GLFW_KEY_RIGHT);
		if (state == GLFW_PRESS)
			return 2;

		//left
		state = glfwGetKey(window, GLFW_KEY_LEFT);
		if (state == GLFW_PRESS)
			return 3;

		//select
		state = glfwGetKey(window, GLFW_KEY_ENTER);
		if (state == GLFW_PRESS)
			return 4;

		//go back
		state = glfwGetKey(window, GLFW_KEY_B);
		if (state == GLFW_PRESS)
			return 5;

		//mystery
		state = glfwGetKey(window, GLFW_KEY_N);
		if (state == GLFW_PRESS)
			return 6;

		return -1;
	//}
}

//displays title screen
bool TitleScreen::DisplayTitle(GLFWwindow *window, XboxController *controller, Audio *audio, int &skyboxIndex, int &arenaIndex, vector<int> *humanVehicleChoice, int &numPlayers) {
	//humanVehicleChoice->clear();
	vector <XboxController> controllers;
	/*int controllerTimeoutCounter = 10, playersNotAdded = 0;
	Timer controllerTimeout;*/
	if (!audio->InitMusic(titleMusic.c_str())) {
		cout << "Failed to init title music." << endl;
	}
	if (!audio->PlayMusic()) {
		cout << "Failed to play music" << endl;
	}
	isLoadScreen = false;
	isMultiplayerScreen = false;
	menuIndex = 0;
	int maxIndex = 2, initIndex = 0;
	//initialize title screen
	InitializeTitleScreen();

	//loop until something happens
	while (!isStartPressed()) {
		//title screen
		if (!isLoadScreen && !isMultiplayerScreen && !isCarScreen) {
			switch (KeyCallback(window, controller, audio)) {	//check key callback 
			case 0:
				toggleMenuIndex(-1, audio, initIndex, maxIndex);
				break;
			case 1:
				toggleMenuIndex(1, audio, initIndex, maxIndex);
				break;
			case 4:
				switch (menuIndex)
				{
				case 0:
					audio->PlaySfx(press, MIX_MAX_VOLUME,1);
					//isLoadScreen = true;
					//destroy menu buttons
					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();

					//init load screen
					InitializeMultiplayerScreen();
					isMultiplayerScreen = true;

					newMenuIndex(multiplayerInitIndex, multiplayerInitIndex, initIndex, maxIndex, 3);
					Sleep(150);		//slow down input so not crazy fast
					break;
				case 1:
					readRules(window, controller, audio,skyboxIndex, arenaIndex, humanVehicleChoice, numPlayers);	//read rules (not implemented yet)

					break;
				case 2:
					pressQuit();	//quit game
					break;
				}
				break;
			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,1);
				break;
			}
		}
		//multiplayer screen
		else if (isMultiplayerScreen) {
			switch (KeyCallback(window, controller, audio)) {	//check key callback 
			case 2:
				toggleMenuIndex(1, audio, initIndex, maxIndex);
				break;
			case 3:
				toggleMenuIndex(-1, audio, initIndex, maxIndex);
				break;
				//press "select"
			case 4:
				audio->PlaySfx(press, MIX_MAX_VOLUME,1);
				//set flag to false
				isMultiplayerScreen = false;
				isCarScreen = true;

				//get arena index (location of button in menuButtons vector -2)
				menuButtons[menuIndex].setColour(pressColour);	//indicate choice
				menuButtons[menuIndex].setMixFlag(1);

				//display colour
				Render();		//render titlescreen
				glfwSwapBuffers(window);	//need this to output to screen
				Sleep(400);		//slow down input so not crazy fast

				for (int i = 0; i < menuButtons.size(); i++)
					menuButtons[i].Destroy();
				InitializeCarScreen();

				numPlayers = menuIndex;
				humanVehicleChoice->clear();
				controllers.clear();
				//	if (controller->Connected())
				// There will always be a Player 1, so push back regardless of whether controller is connected or not;
				// they will be a keyboard otherwise.
				controllers.push_back(*controller);
				for (int i = 0; i < numPlayers; i++) {
					humanVehicleChoice->emplace_back();
				}
				if (numPlayers > 1) {
					//push back rest of controllers
					for (int i = 1; i < numPlayers; i++)
						controllers.push_back(XboxController(i + 1));
				}
				controllerIndex = 0;
				//controllerTimeoutCounter = 10, playersNotAdded = 0;
				newMenuIndex(carButtonInitIndex, carButtonInitIndex, initIndex, maxIndex, 2);

				break;
				//press "back"
			case 5:
				audio->PlaySfx(back, MIX_MAX_VOLUME,1);
				DisplayTitle(window, controller, audio, skyboxIndex, arenaIndex, humanVehicleChoice, numPlayers);
				break;

			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,1);
				break;

			}
		}
		//choose cars
		else if (isCarScreen) {
			switch (KeyCallback(window, &controllers[controllerIndex], audio)) {	//check key callback 
			case 2:
				toggleMenuIndex(1, audio, initIndex, maxIndex);
				break;
			case 3:
				toggleMenuIndex(-1, audio, initIndex, maxIndex);
				break;
				//press "select"
			case 4:
				audio->PlaySfx(press, MIX_MAX_VOLUME,1);
				//set flag to false
				humanVehicleChoice->at(controllerIndex) = (menuIndex - carButtonInitIndex);
				menuButtons[menuIndex].setColour(pressColour);	//indicate choice
				menuButtons[menuIndex].setMixFlag(1);

				controllerIndex++;
				if (controllerIndex >= numPlayers) {
					isCarScreen = false;
					isLoadScreen = true;
					isChooseArena = true;

					//display colour
					Render();		//render titlescreen
					glfwSwapBuffers(window);	//need this to output to screen
					Sleep(400);		//slow down input so not crazy fast

					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();
					InitializeChooseScreen();

					newMenuIndex(arenaButtonInitIndex, arenaButtonInitIndex, initIndex, maxIndex, 2);
				}
				else {
					Sleep(400);
					std::stringstream fmt;
					fmt << "Player " << controllerIndex + 1 << ", choose your vehicle:";
					menuButtons[menuIndex].setColour(selectColour);	//indicate choice
					menuButtons[0].UpdateGameText(fmt.str());
					//controllerTimeoutCounter = 10;
				}

				break;
				//press "back"
			case 5:
				//if (controllers[controllerIndex].GetIndex() == 0) {
				audio->PlaySfx(back, MIX_MAX_VOLUME,1);
				for (int i = 0; i < menuButtons.size(); i++)
					menuButtons[i].Destroy();

				//init load screen
				InitializeMultiplayerScreen();
				isMultiplayerScreen = true;
				isCarScreen = false;
				newMenuIndex(multiplayerInitIndex, multiplayerInitIndex, initIndex, maxIndex, 3);
			//	}
				break;

			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,1);
				break;
			}
		}
		//	}
		//load screen
		else {
			switch (KeyCallback(window, controller, audio)) {	//check key callback 
			case 2:
				toggleMenuIndex(1, audio, initIndex, maxIndex);
				break;
			case 3:
				toggleMenuIndex(-1, audio, initIndex, maxIndex);
				break;
				//press "select"
			case 4:
				audio->PlaySfx(press, MIX_MAX_VOLUME,1);
				//if you choose arena
				if (isChooseArena) {
					//set flag to false
					isChooseArena = false;
					isChooseSkybox = true;
					//get arena index (location of button in menuButtons vector -2)
					arenaIndex = menuIndex - arenaButtonInitIndex;
					//set to colour to indicate choice
					menuButtons[menuIndex].setColour(pressColour);
					newMenuIndex(skyboxButtonInitIndex, skyboxButtonInitIndex, initIndex, maxIndex, 3);

				}
				//choosing skybox
				else if (isChooseSkybox) {
					isChooseSkybox = false;
					//skybox chosen, so get index from menuButton vector 
					skyboxIndex = menuIndex - skyboxButtonInitIndex;
					menuButtons[menuIndex].setColour(pressColour);	//indicate choice
					menuButtons[menuIndex].setMixFlag(1);
					pressStart(audio);	//start game
				}

				break;
				//press "back"
			case 5:
				audio->PlaySfx(back, MIX_MAX_VOLUME,1);
				//reset to choose arena if choosing skybox
				if (!isChooseSkybox && !isChooseArena) {
					//reset flag
					isChooseSkybox = true;
					//unset skybox choice colours
					menuButtons[menuIndex].setMixFlag(0);

					//reset menu index for arenas
					int newMenInd = skyboxIndex + skyboxButtonInitIndex;
					newMenuIndex(newMenInd, skyboxButtonInitIndex, initIndex, maxIndex, 3);

				}
				//reset to choosing arena
				else if (!isChooseArena && isChooseSkybox) {
					//reset flag
					isChooseArena = true;
					//unset skybox choice colours
					menuButtons[menuIndex].setMixFlag(0);

					//reset menu index for arenas
					int newMenInd = arenaIndex + arenaButtonInitIndex;
					newMenuIndex(newMenInd, arenaButtonInitIndex, initIndex, maxIndex, 2);
				}
				//otherwise, go back to title page
				else {
					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();

					//init load screen
					InitializeCarScreen();
					isCarScreen = true;
					isLoadScreen = false;

					newMenuIndex(carButtonInitIndex, carButtonInitIndex, initIndex, maxIndex, 2);
					Sleep(150);		//slow down input so not crazy fast				}
				}
				break;

			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,1);
				break;

			}
		}
		// clear screen to a dark grey colour;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render();		//render titlescreen
		glfwSwapBuffers(window);	//need this to output to screen

		//if quit selected, or other input to close program selected, close program
		if (isQuitPressed() || glfwWindowShouldClose(window)) {
			return 0;
		}
		Sleep(100);		//slow down input so not crazy fast
		glfwPollEvents();
	}
	Sleep(600);
	return 1;
	//Initialize "Loading screen"
}



void TitleScreen::newMenuIndex(int &newMenuIndex, const int &newInitIndex, int &initIndex, int &maxIndex, const int &addIndex) {
	//new menu index to choose scenery/skybox
	initIndex = newInitIndex;
	maxIndex = newInitIndex + addIndex;
	menuIndex = newMenuIndex;
	menuButtons[menuIndex].setColour(selectColour);	//set initial choice colour to green
													//sex mixColour flag
	menuButtons[menuIndex].setMixFlag(1);
}

//clean up
void TitleScreen::Destroy() {
	background.Destroy();
	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].Destroy();
	//delete click;
	//delete press;
	//delete back;
}
