#include "Game\TitleScreen.h"

//TitleScreen cpp file.  Can definitely change around.



TitleScreen::TitleScreen()
{
	isStart = false;
	isRules = false;
	isQuit = false;
	isMode = false;
	isChooseArena = false;
	isChooseSkybox = false;
	isArenaSkyboxScreen = false;
	isCarScreen = false;

	menuIndex = 0;
	modeInitIndex = 6;
	multiplayerInitIndex = 1;
	arenaButtonInitIndex = 2;
	skyboxButtonInitIndex = 5;
	carButtonInitIndex = 1;
	controllerIndex = 0;

	numMenuButtons = 3;

	pauseTime = 100;

	selectColour = vec3(1,0.5,0.5);
	pressColour = vec3(1, 0, 1);
	prevColour = vec3(0);

	click = Mix_LoadWAV("sfx/keyClick.wav");
	rev1 = Mix_LoadWAV("sfx/rev1.wav");
	rev2 = Mix_LoadWAV("sfx/rev2.wav");
	rev3 = Mix_LoadWAV("sfx/rev3.wav");
	rev4 = Mix_LoadWAV("sfx/rev4.wav");
	press = Mix_LoadWAV("sfx/startNoise.wav");
	back = Mix_LoadWAV("sfx/slowDown.wav");

	clickChannel = 1;
	backChannel = 2;
	revChannel1 = 3;
	revChannel2 = 4;
}


TitleScreen::~TitleScreen()
{
	Destroy();
}

//toggle startt boolean
void TitleScreen::pressStart(Audio *audio) {
	isStart = true;
	audio->PlaySfx(press, MIX_MAX_VOLUME,revChannel1);
}

//read rules (not implemented yet)
void TitleScreen::readRules(GLFWwindow *window, XboxController *ctrller, Audio *audio, int &skyboxIndex, int &arenaIndex, vector<int> *humanVehicleChoice, int &numPlayers, int & mode)
{
	ScreenOverlay rulesScreen;
	ScreenOverlay rulesBackText;

	rulesBackText.InitializeGameText("Press B to go back", vec3(-0.15,0.1,0), vec3(0,0,1), 30);
	rulesBackText.setScale(vec3(0.5));
	if (!rulesScreen.initTexture("textures/rulesScreen.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init rules screen." << endl;
	}
	rulesScreen.GenerateSquareVertices(1, 1, vec3(0,0,0));

	rulesScreen.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	rulesBackText.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	int t = 0;
	while (!glfwWindowShouldClose(window)) {

		int key = KeyCallback(window, ctrller, audio);
		if (key ==5) {
			audio->PlaySfx(click, MIX_MAX_VOLUME, clickChannel);
			rulesScreen.Destroy();
			rulesBackText.Destroy();
			return;
		}rulesBackText.setTransparency(float(t % 20)*0.1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		background.Render(GL_TRIANGLE_STRIP, background.getColour());	//render "loading screen"
		rulesScreen.Render(GL_TRIANGLE_STRIP, rulesScreen.getColour());	//render text, as it uses triangles, not triangle strip
		rulesBackText.Render(GL_TRIANGLES, rulesBackText.getColour());
		glfwSwapBuffers(window);	//need this to output to screen
		Sleep(pauseTime);		//slow down input so not crazy fast
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
	audio->PlaySfx(click, MIX_MAX_VOLUME,clickChannel);
}

//Initializes main screen
void TitleScreen::InitializeTitleScreen() {
	/*if (!timer.isStarted())
		timer.start();
	else
		timer.reset();
	*/
	//clear vector of menu buttons
	menuButtons.clear();

	//vector of buttons
	for (int i = 0; i < 6; i++)
		menuButtons.emplace_back();

	//set colours of buttons for now
	menuButtons[0].setColour(selectColour);	//is set to green initially

	float menuXPosition = -0.6;
	float menuYPosition = -0.2;
	menuButtons[0].InitializeGameText("START", vec3(menuXPosition, menuYPosition,0), selectColour, 20);
	menuButtons[1].InitializeGameText("CONTROLS", vec3(menuXPosition, menuYPosition -0.1,0), vec3(0.2,0.2,0), 20);
	menuButtons[2].InitializeGameText("QUIT", vec3(menuXPosition, menuYPosition -0.2, 0), vec3(0.2,0.2,0), 20);

	menuButtons[3].InitializeGameText("DERBY LEAGUE", vec3(-0.6, 0.6, 0), vec3(0, 0, 0), 20);
	menuButtons[4].InitializeGameText("OF DESTRUCTION", vec3(-0.7, 0.3, 0), vec3(0, 0, 0), 20);

	menuButtons[3].setScale(vec3(2));
	menuButtons[4].setScale(vec3(2));

	menuButtons[5].GenerateSquareVertices(0.25, 0.25, vec3(1));
	menuButtons[5].initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D);
	menuButtons[5].setPosition(vec3(0.4, -0.1, 0));

	//initialize which button cursor will be on upon starting
	menuButtons[0].setMixFlag(1); //init
	prevColour = vec3(0.2, 0.2, 0);	//colour for first button

	//generate vertices/scale
	background.GenerateSquareVertices(1, 1, vec3(51.f/255.f, 1.f, 153.f/255.f));	//invalid_enum occurs here

	float buttonWidth = 0.2;
	float buttonHeight = 0.1;
	float padding = 0.05;														//gererate for buttons
	//for (int i = 0; i < menuButtons.size() - 2; i++) {
	//	menuButtons[i].GenerateSquareVertices(buttonWidth, buttonHeight, menuButtons[i].getColour());
	//}

	//set positions of buttons
//	menuButtons[0].setPosition(vec3(0, 0, 0));
//	menuButtons[1].setPosition(vec3(0, -(buttonHeight * 2 + padding), 0));
//	menuButtons[2].setPosition(vec3(0, -(buttonHeight * 4 + padding * 2), 0));

	//initialize shaders for background and buttons
	background.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

void TitleScreen::InitializeMultiplayerScreen() {
	//clear vector of menu buttons
	menuButtons.clear();

	int max_buttons = 8;
	//vector of buttons
	for (int i = 0; i < max_buttons; i++) {
		menuButtons.emplace_back();
	}

	//screen overlays for number of players
	menuButtons[0].InitializeGameText("How many players?", vec3(-0.6, 0.4, 0), vec3(0), 30);
	menuButtons[0].setScale(vec3(2.f));
	for (int i = 1; i < 5; i++) {
		menuButtons[i].InitializeGameText(to_string(i), vec3(-0.6 + (2 * i*0.1), -0.2, 0), vec3(0, 0, 0), 20);
		menuButtons[i].setScale(vec3(3.f));

	}
	menuButtons[5].InitializeGameText("Choose mode:", vec3(-0.4, -0.4, 0), vec3(0), 30);
	menuButtons[6].InitializeGameText("Timed", vec3(-0.6, -0.6, 0), vec3(0), 30);
	menuButtons[7].InitializeGameText("Sudden Death", vec3(0.0, -0.6, 0), vec3(0), 30);

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

	//vector of buttons
	for (int i = 0; i <4; i++) {
		menuButtons.emplace_back();
	}
	menuButtons[0].InitializeGameText("Player 1, choose your vehicle:", vec3(-0.5, 0.3, 0), vec3(0), 30);

	//vehicle choice buttons
	if (!menuButtons[carButtonInitIndex].initTexture("textures/carImgs/lightCar.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init light car texture." << endl;
	}
	if (!menuButtons[carButtonInitIndex + 1].initTexture("textures/carImgs/mediumCar.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init medium car texture." << endl;
	}
	if (!menuButtons[carButtonInitIndex + 2].initTexture("textures/carImgs/heavyCar.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init heavy car texture." << endl;
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

	//vector of buttons
	for (int i = 0; i <9; i++) {
		menuButtons.emplace_back();
	}

	//screen overlays for arena and location
	menuButtons[0].InitializeGameText("Choose your arena", vec3(-0.4, 0.7, 0), vec3(0), 20);
	menuButtons[1].InitializeGameText("Choose your location", vec3(-0.5, 0.0, 0), vec3(0), 20);

	//init transparent arena textures
	if (!menuButtons[arenaButtonInitIndex].initTexture("textures/arenaImgs/bowl.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init bowl texture." << endl;
	}
	if (!menuButtons[arenaButtonInitIndex + 1].initTexture("textures/arenaImgs/devil.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init devil texture." << endl;
	}
	if (!menuButtons[arenaButtonInitIndex + 2].initTexture("textures/arenaImgs/wrap.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init wrap texture." << endl;
	}

	//initialize which button cursor will be on upon starting
	menuButtons[arenaButtonInitIndex].setColour(selectColour);	//is set to green initially
	menuButtons[arenaButtonInitIndex].setMixFlag(1); //init
	prevColour = vec3(1, 1, 0);	//colour for first button

								//textures for skybox choice buttons
	if (!menuButtons[skyboxButtonInitIndex].initTexture("textures/ely_cloudtop/cloudtop_bk.tga", GL_TEXTURE_2D)) {
	//	cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex + 1].initTexture("textures/nec_hell/hell_lf.tga", GL_TEXTURE_2D)) {
	//	cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex + 2].initTexture("textures/mp_deviltooth/devils-tooth_lf.tga", GL_TEXTURE_2D)) {
	//	cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex + 3].initTexture("textures/mp_emerald/emerald-summit_lf.tga", GL_TEXTURE_2D)) {
	//	cout << "Failed to init bpArena2 texture." << endl;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
bool TitleScreen::DisplayTitle(GLFWwindow *window, XboxController *controller, Audio *audio, int &skyboxIndex, int &arenaIndex, vector<int> *humanVehicleChoice, int &numPlayers, int & mode) {
	vector <XboxController> controllers;
	if (!audio->InitMusic(titleMusic.c_str())) {
	//	cout << "Failed to init title music." << endl;
	}
	if (!audio->PlayMusic()) {
	//	cout << "Failed to play music" << endl;
	}
	isArenaSkyboxScreen = false;
	isMultiplayerScreen = false;
	isMode = false;
	menuIndex = 0;
	int maxIndex = 2, initIndex = 0;
	//initialize title screen
	InitializeTitleScreen();

	//loop until something happens
	while (!isStartPressed()) {
		//title screen
		if (!isArenaSkyboxScreen && !isMultiplayerScreen && !isMode && !isCarScreen) {
			switch (KeyCallback(window, controller, audio)) {	//check key callback 
			case 0:
				toggleMenuIndex(-1, audio, initIndex, maxIndex);
				break;
			case 1:
				toggleMenuIndex(1, audio, initIndex, maxIndex);
				break;
			case 4:
			//	if (!timer.isStopped()) {
			//		timer.stop();
			//	}
				switch (menuIndex)
				{
				case 0:
					audio->PlaySfx(rev1, MIX_MAX_VOLUME, revChannel2);
					//destroy menu buttons
					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();

					//init load screen
					InitializeMultiplayerScreen();
					isMultiplayerScreen = true;

					newMenuIndex(multiplayerInitIndex, multiplayerInitIndex, initIndex, maxIndex, 3);
					Sleep(pauseTime*2.5);		//slow down input so not crazy fast
					break;
				case 1:
					audio->PlaySfx(click, MIX_MAX_VOLUME, clickChannel);
					readRules(window, controller, audio,skyboxIndex, arenaIndex, humanVehicleChoice, numPlayers, mode);
					break;
				case 2:
					pressQuit();	//quit game
					break;
				}
				break;
			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,revChannel2);
				break;
			}
		}
		//multiplayer screen
		else if (isMultiplayerScreen || isMode) {
			switch (KeyCallback(window, controller, audio)) {	//check key callback 
			case 2:
				toggleMenuIndex(1, audio, initIndex, maxIndex);
				break;
			case 3:
				toggleMenuIndex(-1, audio, initIndex, maxIndex);
				break;
				//press "select"
			case 4:
				audio->PlaySfx(rev2, MIX_MAX_VOLUME,revChannel1);
				//set flag to false
				//isMultiplayerScreen = false;

				if (!isMode) {
					isMultiplayerScreen = false;
					isMode = true;
					//get arena index (location of button in menuButtons vector -2)
					menuButtons[menuIndex].setColour(pressColour);	//indicate choice
					menuButtons[menuIndex].setMixFlag(1);


					numPlayers = menuIndex;
					humanVehicleChoice->clear();
					controllers.clear();

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

					newMenuIndex(modeInitIndex, modeInitIndex, initIndex, maxIndex, 1);


					//display colour
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					Render();		//render titlescreen
					glfwSwapBuffers(window);	//need this to output to screen
					Sleep(pauseTime*2.5);		//slow down input so not crazy fast
				}
				else {
					mode = menuIndex - modeInitIndex;

					isMode = false;
					isMultiplayerScreen = false;
					isCarScreen = true;
					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();
					InitializeCarScreen();

					newMenuIndex(carButtonInitIndex, carButtonInitIndex, initIndex, maxIndex, 2);
				}

				break;
				//press "back"
			case 5:
				if (isMode) {
					audio->PlaySfx(back, MIX_MAX_VOLUME, backChannel);
					menuButtons[menuIndex].setMixFlag(0);

					isMode = false;
					isMultiplayerScreen = true;
					newMenuIndex(multiplayerInitIndex, multiplayerInitIndex, initIndex, maxIndex, 3);

					Sleep(pauseTime *2.5);
					/*


					//reset menu index for arenas
					newMenuIndex(newMenInd, skyboxButtonInitIndex, initIndex, maxIndex, 3);
					Sleep(pauseTime * 2.5);		//slow down input so not crazy fast				}*/
				}

				else {
					audio->PlaySfx(back, MIX_MAX_VOLUME, backChannel);
					DisplayTitle(window, controller, audio, skyboxIndex, arenaIndex, humanVehicleChoice, numPlayers, mode);
				}
				break;

			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,revChannel1);
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
				audio->PlaySfx(rev3, MIX_MAX_VOLUME,revChannel2);
				//set flag to false
				humanVehicleChoice->at(controllerIndex) = (menuIndex - carButtonInitIndex);
				menuButtons[menuIndex].setColour(pressColour);	//indicate choice
				menuButtons[menuIndex].setMixFlag(1);

				Render();		//render titlescreen
				glfwSwapBuffers(window);	//need this to output to screen
				Sleep(300);		//slow down input so not crazy fast

				controllerIndex++;
				if (controllerIndex >= numPlayers) {
					isCarScreen = false;
					isArenaSkyboxScreen = true;
					isChooseArena = true;

					//display colour
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					Render();		//render titlescreen
					glfwSwapBuffers(window);	//need this to output to screen
					Sleep(300);		//slow down input so not crazy fast

					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();
					InitializeChooseScreen();

					newMenuIndex(arenaButtonInitIndex, arenaButtonInitIndex, initIndex, maxIndex, 2);
				}

				else {
					Sleep(pauseTime * 2.5);
					std::stringstream fmt;
					fmt << "Player " << controllerIndex + 1 << ", choose your vehicle:";
					menuButtons[menuIndex].setColour(selectColour);	//indicate choice
					menuButtons[0].UpdateGameText(fmt.str(), 30);
				}

				break;
				//press "back"
			case 5:
				audio->PlaySfx(back, MIX_MAX_VOLUME,backChannel);
				for (int i = 0; i < menuButtons.size(); i++)
					menuButtons[i].Destroy();

				//init load screen
				InitializeMultiplayerScreen();
				isMultiplayerScreen = true;
				isCarScreen = false;
				newMenuIndex(multiplayerInitIndex, multiplayerInitIndex, initIndex, maxIndex, 3);
				Sleep(pauseTime * 2.5);		//slow down input so not crazy fast				}

			//	}
				break;

			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,revChannel2);
				break;
			}
		}
		//	}
		//choose arena and skybox
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
				audio->PlaySfx(rev4, MIX_MAX_VOLUME,revChannel1);
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
					Sleep(pauseTime * 2.5);		//slow down input so not crazy fast				}
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
				audio->PlaySfx(back, MIX_MAX_VOLUME,backChannel);
				//reset to choose arena if choosing skybox
				if (!isChooseSkybox && !isChooseArena) {
					//reset flag
					isChooseSkybox = true;
					//unset skybox choice colours
					menuButtons[menuIndex].setMixFlag(0);

					//reset menu index for arenas
					int newMenInd = skyboxIndex + skyboxButtonInitIndex;
					newMenuIndex(newMenInd, skyboxButtonInitIndex, initIndex, maxIndex, 3);
					Sleep(pauseTime * 2.5);		//slow down input so not crazy fast				}


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
					Sleep(pauseTime * 2.5);		//slow down input so not crazy fast				}

				}
				//otherwise, go back to title page
				else {
					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();

					//init load screen
					InitializeCarScreen();
					isCarScreen = true;
					isArenaSkyboxScreen = false;
					isChooseArena = false;
					controllerIndex = 0;

					newMenuIndex(carButtonInitIndex, carButtonInitIndex, initIndex, maxIndex, 2);
					Sleep(pauseTime * 2.5);		//slow down input so not crazy fast				}
				}
				break;

			case 6:
				audio->PlaySfx(press, MIX_MAX_VOLUME,revChannel1);
				break;

			}
		}
		// clear screen to a dark grey colour;
		Render();		//render titlescreen
		glfwSwapBuffers(window);	//need this to output to screen

		//if quit selected, or other input to close program selected, close program
		if (isQuitPressed() || glfwWindowShouldClose(window)) {
			return 0;
		}
		Sleep(pauseTime * 1.5);		//slow down input so not crazy fast
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
	//if (!timer.isStopped())
	//	timer.stop();
	//delete click;
	//delete press;
	//delete back;
}
