#include "Game\TitleScreen.h"

//TitleScreen cpp file.  Can definitely change around.



TitleScreen::TitleScreen()
{
	pauseTime = 100;
}

TitleScreen::~TitleScreen()
{
	Destroy();
}

//toggle startt boolean
void TitleScreen::pressStart(Audio *audio) {
	isStart = true;
	audio->PlaySfx("chicken", MIX_MAX_VOLUME,select);
}

//quit game
void TitleScreen::pressQuit() {
	isQuit = true;
	glfwSetWindowShouldClose(window, true);
}

//render all components
void TitleScreen::Render() {
	if (currentScreen != NULL)
		currentScreen->Render();

}

int TitleScreen::Run(vector<int> &humanVehicleChoice, int &numPlayers, int & mode) {
	currentScreen->Run();
	if (!currentScreen->checkVisible()) {
		switch (currentScreen->getScreenIndex()) {
			//main page
		case 0: {
			MainScreen *m = dynamic_cast<MainScreen*> (currentScreen);
			if (m != nullptr) {
				if (m->checkRules()) {
					currentScreen->Destroy();
					currentScreen = new RulesScreen(window, controller, audio);
					currentScreen->Initialize();
				}
				else if (m->checkQuit())
					pressQuit();
				else {
					currentScreen->Destroy();
					currentScreen = new MultiplayerScreen(window, controller, audio);
					currentScreen->Initialize();
				}
			}
		}
				break;
				//rules
		case 1:
			currentScreen->Destroy();
			currentScreen = new MainScreen(window, controller, audio);
			currentScreen->Initialize();
			break;
			//multiplayer choice
		case 2:
			if (currentScreen->goBack()) {
				currentScreen->Destroy();
				currentScreen = new MainScreen(window, controller, audio);
				currentScreen->Initialize();
			}
			else {
				numPlayers = currentScreen->returnIndex();
				//cout << numPlayers << endl;
				controllers.clear();
				controllers.push_back(*controller);
				for (int i = 0; i < numPlayers; i++) {
					humanVehicleChoice.emplace_back();
				}
				if (numPlayers > 1) {
					//push back rest of controllers
					for (int i = 1; i < numPlayers; i++)
						controllers.push_back(XboxController(i + 1));
				}
				currentScreen->Destroy();
				currentScreen = new ModeScreen(window, controller, audio);
				currentScreen->Initialize();
			}
			break;
			//mode choice
		case 3:
			if (currentScreen->goBack()) {
				currentScreen->Destroy();
				currentScreen = new MultiplayerScreen(window, controller, audio);
				currentScreen->Initialize();
			}
			else {
				mode = currentScreen->returnIndex();
				currentScreen->Destroy();
				currentScreen = new CarScreen(window, controllers, audio, numPlayers);
				currentScreen->Initialize();
				vehicleIndex = 0;
			}
			break;
			//car choice
		case 4:
			if (currentScreen->goBack()) {
				currentScreen->Destroy();
				currentScreen = new ModeScreen(window, controller, audio);
				currentScreen->Initialize();
			}
			else {
				humanVehicleChoice[vehicleIndex++] = currentScreen->returnIndex();
				if (vehicleIndex < numPlayers) {
					currentScreen->isVisible = true;
				}
				else {
					isStart = true;
				}
			}
			break;
		default:
			break;
		}
	}

	// clear screen to a dark grey colour;
	Render();		//render titlescreen
	glfwSwapBuffers(window);	//need this to output to screen

	if (isStart)
		return 0;
	Sleep(pauseTime * 1.5);		//slow down input so not crazy fast
	glfwPollEvents();
	return 1;
}

//displays title screen
bool TitleScreen::Initialize(GLFWwindow *w, XboxController *c, Audio *a) {
	this->window = w;
	this->controller = c;
	this->audio = a;

	isStart = false;
	isQuit = false;

	vehicleIndex = 0;
	if (!audio->InitMusic(titleMusic.c_str())) {
		cout << "Failed to init title music." << endl;
		return false;
	}

	if (!audio->PlayMusic()) {
		cout << "Failed to play music" << endl;
		return false;
	}

	//initialize title screen
	currentScreen = new MainScreen(window, controller, audio);
	currentScreen->Initialize();
	return true;
	//loop until something happens
}

//clean up
void TitleScreen::Destroy() {
	currentScreen->Destroy();
}
