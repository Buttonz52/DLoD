#include "..\headers\Game\Title\RulesScreen.h"



RulesScreen::RulesScreen() :Screen()
{
	pauseTime = 100;
	timeCounter = 0;
	screenIndex = 1;
	kerning = 30;
};

RulesScreen::RulesScreen(GLFWwindow *w, XboxController *x, Audio *a) : Screen(w,x,a, vec3()){
	pauseTime = 100;
	timeCounter = 0;
	screenIndex = 1;
	kerning = 30;

};
RulesScreen::~RulesScreen()
{
	Destroy();
}

void RulesScreen::Initialize() {
	for (int i = 0; i < 2; i++) {
		menuButtons.emplace_back();
	}
	menuButtons[1].InitializeGameText("Press B to go back", vec3(-0.15, 0.1, 0), vec3(0, 0, 1), kerning);
	menuButtons[1].SetScale(vec3(0.5));
	if (!menuButtons[0].InitTexture("textures/rulesScreen.png", GL_TEXTURE_2D)) {
		cout << "Failed to init rules screen." << endl;
	}
	menuButtons[0].GenerateSquareVertices(1, 1, vec3(0, 0, 0));

	menuButtons[0].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	menuButtons[1].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

void RulesScreen::Run() {
	int key = KeyCallback(window, controller, audio);
	if (key ==5) {
		audio->PlaySfx("back", MIX_MAX_VOLUME, menu);
		isVisible = false;
		return;
	}menuButtons[1].SetTransparency(float(timeCounter % 20)*0.1);
	//Sleep(pauseTime);		//slow down input so not crazy fast
	timeCounter++;
}