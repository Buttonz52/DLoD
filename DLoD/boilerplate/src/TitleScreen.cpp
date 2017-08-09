#include "Game\TitleScreen.h"

//TitleScreen cpp file.  Can definitely change around.



TitleScreen::TitleScreen()
{
	pauseTime = 150;
	textColour = vec3();
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::resetScreen() {
	delete currentScreen;
}
//toggle startt boolean
void TitleScreen::pressStart(Audio *audio) {
	isStart = true;
	audio->PlaySfx("chicken", MIX_MAX_VOLUME,select);
	Sleep(pauseTime * 3);
}

//quit game
void TitleScreen::pressQuit() {
	isQuit = true;
	glfwSetWindowShouldClose(window, true);
}

//displays title screen
bool TitleScreen::Initialize(GLFWwindow *w, XboxController *c, Audio *a) {
	this->window = w;
	this->controller = c;
	this->audio = a;

	isStart = false;
	isQuit = false;

	if (!timer.isStarted())
		timer.start();
	vehicleIndex = 0;
	if (!audio->InitMusic(titleMusic.c_str())) {
		cout << "Failed to init title music." << endl;
		return false;
	}

	if (!audio->PlayMusic()) {
		cout << "Failed to play music" << endl;
		return false;
	}

	//initialize main screen
	currentScreen = new MainScreen(window, controller, audio, textColour);
	currentScreen->Initialize();
	timer.startSleep(pauseTime * 4);
	return true;
}

void TitleScreen::UpdateColour(const vec3 &newColour) {
	currentScreen->UpdateColour(newColour);
}
//render all components
void TitleScreen::Render() {
	if (currentScreen != NULL)
		currentScreen->Render();
}

int TitleScreen::Run(vector<int> &humanVehicleChoice, int &numPlayers, int & mode, vec3 &newColour) {
	textColour = newColour;
	if (!timer.checkSleep()) {
		currentScreen->Run();
		timer.startSleep(pauseTime);
		if (!currentScreen->checkVisible()) {
			switch (currentScreen->getScreenIndex()) {
				//main page
			case 0: {
				MainScreen *m = dynamic_cast<MainScreen*> (currentScreen);
				if (m != nullptr) {
					if (m->checkRules()) {
						currentScreen->Destroy();
						resetScreen();
						currentScreen = new RulesScreen(window, controller, audio);
						currentScreen->Initialize();
						timer.startSleep(pauseTime * 4);
					}
					else if (m->checkQuit())
						pressQuit();
					else {
						InitMultiplayerScreen(window, controller, audio, textColour);
					}
				}
			}
					break;
					//rules
			case 1:
				InitMainScreen(window, controller, audio, textColour);
				break;
				//multiplayer choice
			case 2:
				if (currentScreen->goBack()) {
					InitMainScreen(window, controller, audio, textColour);
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
					InitModeScreen(window, controller, audio, textColour);
				}
				break;
				//mode choice
			case 3:
				if (currentScreen->goBack()) {
					InitMultiplayerScreen(window, controller, audio, textColour);
				}
				else {
					mode = currentScreen->returnIndex();
					InitCarScreen(window, controllers, audio, numPlayers, textColour);

				}
				break;
				//car choice
			case 4:
				if (currentScreen->goBack()) {
					InitModeScreen(window, controller, audio, textColour);
				}
				else {
					humanVehicleChoice[vehicleIndex++] = currentScreen->returnIndex();
					if (vehicleIndex < numPlayers) {
						currentScreen->isVisible = true;
					}
					else {
						currentScreen->Destroy();
						resetScreen();
						currentScreen = new PlayButtonScreen(window, controller, audio, textColour);
						currentScreen->Initialize();
						timer.startSleep(pauseTime * 4);
					}
				}
				break;
			case 5:
				if (currentScreen->goBack()) {
					InitCarScreen(window, controllers, audio, numPlayers, textColour);
				}
				else {
					isStart = true;
				}
				break;
			default:
				break;
			}
			//Sleep(pauseTime*1.5);
		}
	}
	Render();		//render titlescreen
	glfwSwapBuffers(window);	//need this to output to screen

	if (isStart)
		return 0;
	//Sleep(pauseTime * 1.5);		//slow down input so not crazy fast
	glfwPollEvents();
	return 1;
}

//clean up
void TitleScreen::Destroy() {
	currentScreen->Destroy();
	resetScreen();
}

void TitleScreen::InitCarScreen(GLFWwindow *w, vector<XboxController> &c, Audio *a, const int &numPlayers, vec3 &co) {
	currentScreen->Destroy();
	resetScreen();
	currentScreen = new CarScreen(w,c,a, numPlayers, co);
	currentScreen->Initialize();
	timer.startSleep(pauseTime * 4);
	vehicleIndex = 0;
}

void TitleScreen::InitModeScreen(GLFWwindow *w, XboxController *c, Audio *a, vec3 &co) {
	currentScreen->Destroy();
	resetScreen();
	currentScreen = new ModeScreen(w,c,a, co);
	currentScreen->Initialize();
	timer.startSleep(pauseTime * 4);
}

void TitleScreen::InitMainScreen(GLFWwindow *w, XboxController *c, Audio *a, vec3 &co) {
	currentScreen->Destroy();
	resetScreen();
	currentScreen = new MainScreen(w,c,a, co);
	currentScreen->Initialize();
	timer.startSleep(pauseTime * 4);
}
void TitleScreen::InitMultiplayerScreen(GLFWwindow *w, XboxController *c, Audio *a, vec3 &co) {
	currentScreen->Destroy();
	resetScreen();
	currentScreen = new MultiplayerScreen(w, c, a, co);
	currentScreen->Initialize();
	timer.startSleep(pauseTime * 4);
}
