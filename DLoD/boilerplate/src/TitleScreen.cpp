#include "..\headers\Game\TitleScreen.h"

//TitleScreen cpp file.  Can definitely change around.



TitleScreen::TitleScreen()
{
	menuIndex = 0;
	arenaButtonInitIndex = 3;
	skyboxButtonInitIndex = 6;
	carButtonInitIndex = 10;
	isLoadScreen = false;
//	this->audio = audio;
	buttonWidth = 0.2;
	buttonHeight = 0.1;
	padding = 0.05;
	isStart = false;
	isQuit = false;
	isRules = false;
	isChooseSkybox = false;
	isChooseArena = false;
	numMenuButtons = 3;
	selectColour = vec3(0, 1, 0);
	pressColour = vec3(1, 0, 1);
	prevCol = vec3(0);
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
	if (isLoadScreen) {
		//choose arena
		if (isChooseArena) {
			if (menuIndex > 5) {
				menuIndex = 3;
			}
			if (menuIndex < 3) {
				menuIndex = 5;
			}
		}
		//choose skybox
		else if (isChooseSkybox){
			if (menuIndex >9) {
				menuIndex = 6;
			}
			if (menuIndex < 6) {
				menuIndex = 9;
			}
		}
		//choose vehicle
		else {
			if (menuIndex >= menuButtons.size()) {
				menuIndex = 10;
			}
			if (menuIndex < 10) {
				menuIndex = menuButtons.size() - 1;
			}
		}
	}
	else {
		//top to bottom
		if (menuIndex < 0) {
			menuIndex = menuButtons.size() - 3;
		}

		//bottom to top
		if (menuIndex >= menuButtons.size()-2) {
			menuIndex = 0;
		}
	}

	//set the previous colour
	prevCol = menuButtons[menuIndex].getColour();
	//new colour (green for the moment); used to mix with texture for current button (we can change this)
	menuButtons[menuIndex].setColour(vec3(0, 1, 0));
	//sex mixColour flag
	menuButtons[menuIndex].setMixColour(1);
	audio->PlaySfx(click);
	
}

//Initializes main screen
void TitleScreen::InitializeTitleScreen() {

	//clear vector of menu buttons
	menuButtons.clear();

	//generic button with transparent background
	ScreenOverlay button;
	button.isRedTransparent = 1;

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
	menuButtons[0].setMixColour(1); //init
	prevCol = vec3(1, 1, 0);	//colour for first button

//	if (!background.initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
//		cout << "Failed to init texture." << endl;
//	}

	//generate vertices/scale
	background.GenerateSquareVertices(1, 1, vec3(0,0,1));	//invalid_enum occurs here

	//gererate for buttons
	for (int i = 0; i < menuButtons.size()-2; i++) {
		menuButtons[i].GenerateSquareVertices(buttonWidth, buttonHeight, menuButtons[i].getColour());
	}

	//set positions of buttons
	menuButtons[0].setPosition(vec3(0, 0, 0));
	menuButtons[1].setPosition(vec3(0, -(buttonHeight*2+ padding), 0));
	menuButtons[2].setPosition(vec3(0, -(buttonHeight*4 + padding*2), 0));

	//initialize shaders for background and buttons
	background.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

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
	for (int i = 0; i <3 + carButtonInitIndex; i++) {
		menuButtons.push_back(button);
	}

	//screen overlays for arena and location
	menuButtons[0].InitializeGameText("Choose your arena", vec3(-0.4, 0.8, 0), vec3(0, 1, 0), 20);
	menuButtons[1].InitializeGameText("Choose your location", vec3(-0.5, 0.2, 0), vec3(0, 1, 0), 20);
	menuButtons[2].InitializeGameText("Choose your vehicle", vec3(-0.5, -0.4, 0), vec3(0, 1, 0), 20);

	//init transparent arena textures
	if (!menuButtons[arenaButtonInitIndex].initTexture("textures/arenaImgs/arena9.png", GL_TEXTURE_2D)) {
		cout << "Failed to init arena9 texture." << endl;
	}
	if (!menuButtons[arenaButtonInitIndex+1].initTexture("textures/arenaImgs/bpArena2.png", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[arenaButtonInitIndex+2].initTexture("textures/arenaImgs/bumpyArena.png", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	menuButtons[arenaButtonInitIndex].isRedTransparent = 1;
	menuButtons[arenaButtonInitIndex+1].isRedTransparent = 1;
	menuButtons[arenaButtonInitIndex+2].isRedTransparent = 1;


	//initialize which button cursor will be on upon starting
	menuButtons[arenaButtonInitIndex].setColour(selectColour);	//is set to green initially
	menuButtons[arenaButtonInitIndex].setMixColour(1); //init
	prevCol = vec3(1, 1, 0);	//colour for first button

	//textures for skybox choice buttons
	if (!menuButtons[skyboxButtonInitIndex].initTexture("textures/ame_ash/ashcanyon_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex+1].initTexture("textures/jf_nuke/nuke_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex+2].initTexture("textures/mp_deviltooth/devils-tooth_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}
	if (!menuButtons[skyboxButtonInitIndex+3].initTexture("textures/mp_emerald/emerald-summit_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init bpArena2 texture." << endl;
	}

	//vehicle choice buttons
	if (!menuButtons[carButtonInitIndex].initTexture("textures/carImgs/lightCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init light car texture." << endl;
	}
	if (!menuButtons[carButtonInitIndex+1].initTexture("textures/carImgs/mediumCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init medium car texture." << endl;
	}
	if (!menuButtons[carButtonInitIndex+2].initTexture("textures/carImgs/heavyCar.png", GL_TEXTURE_2D)) {
		cout << "Failed to init heavy car texture." << endl;
	}
	menuButtons[carButtonInitIndex].isRedTransparent = 1;
	menuButtons[carButtonInitIndex+1].isRedTransparent = 1;
	menuButtons[carButtonInitIndex+2].isRedTransparent = 1;

	//vertices for background
	background.GenerateSquareVertices(1, 1, vec3(0, 0, 1));	//invalid_enum occurs here

	//gererate for buttons
	for (int i = 3; i < menuButtons.size(); i++) {
		menuButtons[i].GenerateSquareVertices(0.2, 0.2, menuButtons[i].getColour());
	}

	//set positions of buttons
	menuButtons[3].setPosition(vec3(-0.4,0.6,0));
	menuButtons[4].setPosition(vec3(0.0,0.6,0));
	menuButtons[5].setPosition(vec3(0.4, 0.6, 0));

	menuButtons[6].setPosition(vec3(-0.6,0.0,0));
	menuButtons[7].setPosition(vec3(-0.2,0.0,0));
	menuButtons[8].setPosition(vec3(0.2,0.0,0));
	menuButtons[9].setPosition(vec3(0.6,0.0,0));

	menuButtons[10].setPosition(vec3(-0.4,-0.55,0));
	menuButtons[11].setPosition(vec3(0, -0.55, 0));
	menuButtons[12].setPosition(vec3(0.4, -0.55, 0));


	//initialize shaders
	background.InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}
//render all components
void TitleScreen::Render() {
	background.Render(GL_TRIANGLE_STRIP);	//render "loading screen"
	for (int i = 0; i < menuButtons.size(); i++)
		if (!menuButtons[i].isFontTex) {
			menuButtons[i].Render(GL_TRIANGLE_STRIP);	//render regular button
		}
		else {
			menuButtons[i].Render(GL_TRIANGLES);	//render text, as it uses triangles, not triangle strip
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
	//keyboard input
	else {
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
	}
}

//displays title screen
bool TitleScreen::DisplayTitle(GLFWwindow *window, XboxController *controller, Audio *audio, int &skyboxIndex, int &arenaIndex, int &humanVehicleChoice) {
	if (!audio->InitMusic(titleMusic.c_str())) {
		cout << "Failed to init title music." << endl;
	}
	if (!audio->PlayMusic()) {
		cout << "Failed to play music" << endl;
	}
	isLoadScreen = false;
	menuIndex = 0;

	//initialize title screen
	InitializeTitleScreen();

	//loop until something happens
	while (!isStartPressed()) {
		//title screen
		if (!isLoadScreen) {
			switch (KeyCallback(window, controller, audio)) {	//check key callback 
			case 0:
				toggleMenuIndex(-1, audio);
				break;
			case 1:
				toggleMenuIndex(1, audio);
				break;
			case 4:
				switch (menuIndex)
				{
				case 0:
					audio->PlaySfx(press);
					isLoadScreen = true;
					//destroy menu buttons
					for (int i = 0; i < menuButtons.size(); i++)
						menuButtons[i].Destroy();

					//init load screen
					InitializeChooseScreen();
					isChooseArena = true;
					menuIndex = arenaButtonInitIndex;	//going to next loop, reset menuIndex
					Sleep(150);		//slow down input so not crazy fast
					break;
				case 1:
					pressRules();	//read rules (not implemented yet)
					break;
				case 2:
					pressQuit();	//quit game
					break;
				}
				break;
			case 6:
				audio->PlaySfx(press);
				break;
			}
		}
		//load screen
		else {
			switch (KeyCallback(window, controller, audio)) {	//check key callback 
			case 2:
				toggleMenuIndex(1, audio);
				break;
			case 3:
				toggleMenuIndex(-1, audio);
				break;
				//press "select"
			case 4:
				audio->PlaySfx(press);
				//if you choose arena
				if (isChooseArena) {
					//set flag to false
					isChooseArena = false;
					isChooseSkybox = true;
					//get arena index (location of button in menuButtons vector -2)
					arenaIndex = menuIndex - arenaButtonInitIndex;
					//set to colour to indicate choice
					menuButtons[menuIndex].setColour(pressColour);	

					//new menu index to choose scenery/skybox
					menuIndex = skyboxButtonInitIndex;
					menuButtons[skyboxButtonInitIndex].setColour(selectColour);	//set initial choice colour to green
					//sex mixColour flag
					menuButtons[skyboxButtonInitIndex].setMixColour(1);
					Sleep(150);		//slow down input so not crazy fast

				}
				//choosing skybox
				else if (isChooseSkybox){
					isChooseSkybox = false;
					//skybox chosen, so get index from menuButton vector 
					skyboxIndex = menuIndex - skyboxButtonInitIndex;
					menuButtons[menuIndex].setColour(pressColour);	//indicate choice
					menuButtons[menuIndex].setMixColour(1);

					//new menu index to choose scenery/skybox
					menuIndex = carButtonInitIndex;
					menuButtons[carButtonInitIndex].setColour(selectColour);	//set initial choice colour to green
															//sex mixColour flag
					menuButtons[carButtonInitIndex].setMixColour(1);
					Sleep(150);		//slow down input so not crazy fast
					//pressStart(audio);	//start game
				}
				//choosing vehicle
				else {
					humanVehicleChoice = menuIndex - carButtonInitIndex;
					menuButtons[menuIndex].setColour(pressColour);	//indicate choice
					menuButtons[menuIndex].setMixColour(1);
					pressStart(audio);	//start game
				}
				break;
			//press "back"
			case 5:
				audio->PlaySfx(back);
				//reset to choose arena if choosing skybox
				if (!isChooseSkybox) {
					//reset flag
					isChooseSkybox = true;
					//unset skybox choice colours
					menuButtons[menuIndex].setMixColour(0);
					//reset menu index for arenas
					menuIndex = skyboxIndex + skyboxButtonInitIndex;

					//unset choice colour back to pick colour
					menuButtons[menuIndex].setColour(selectColour);

					//sex mixColour flag
					menuButtons[menuIndex].setMixColour(1);
				}
				else if (!isChooseArena) {
					//reset flag
					isChooseArena = true;
					//unset skybox choice colours
					menuButtons[menuIndex].setMixColour(0);
					//reset menu index for arenas
					menuIndex = arenaIndex + arenaButtonInitIndex;

					//unset choice colour back to pick colour
					menuButtons[menuIndex].setColour(selectColour);

					//sex mixColour flag
					menuButtons[menuIndex].setMixColour(1);
				}
				//otherwise, go back to title page
				else {
					DisplayTitle(window, controller, audio, skyboxIndex, arenaIndex, humanVehicleChoice);
				}
				break;

			case 6:
				audio->PlaySfx(press);
				break;

			}
		}
		// clear screen to a dark grey colour;
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render();		//render titlescreen
		glfwSwapBuffers(window);	//need this to output to screen

		//if quit selected, or other input to close program selected, close program
		if (isQuitPressed() || glfwWindowShouldClose(window)) {
			//glfwSetWindowShouldClose(window, GL_TRUE);
			//Destroy();
			//glfwDestroyWindow(window);
			//glfwTerminate();
			return 0;
		}
		Sleep(100);		//slow down input so not crazy fast
		glfwPollEvents();
	}
	audio->FreeMusic();
	Sleep(600);
	return 1;
	//Initialize "Loading screen"
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