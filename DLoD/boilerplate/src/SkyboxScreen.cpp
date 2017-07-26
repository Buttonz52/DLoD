#include "..\headers\Game\Title\SkyboxScreen.h"



SkyboxScreen::SkyboxScreen() :Screen()
{
	maxIndex = 3;
	screenIndex = 6;
	kerning = 30;
};

SkyboxScreen::SkyboxScreen(GLFWwindow * w, XboxController * x, Audio * a) :Screen(w,x,a, vec3())
{
	maxIndex = 3;
	screenIndex = 6;
	kerning = 30;
};

SkyboxScreen::~SkyboxScreen()
{
}

void SkyboxScreen::Initialize()
{
	int numButtons = 5;
	int skyboxIndex = 0;
	//vector of buttons
	for (int i = 0; i <numButtons; i++) {
		menuButtons.emplace_back();
	}
		
	//initialize which button cursor will be on upon starting
	menuButtons[skyboxIndex].SetColour(selectColour);	//is set to green initially
	menuButtons[skyboxIndex].SetMixFlag(1); //init
	prevColour = vec3(1, 1, 0);	//colour for first button
	
	//textures for skybox choice buttons
	if (!menuButtons[skyboxIndex].InitTexture("textures/ely_cloudtop/cloudtop_bk.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init cloud texture." << endl;
	}
	if (!menuButtons[skyboxIndex + 1].InitTexture("textures/nec_hell/hell_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init hell texture." << endl;
	}
	if (!menuButtons[skyboxIndex + 2].InitTexture("textures/mp_deviltooth/devils-tooth_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init devil texture." << endl;
	}
	if (!menuButtons[skyboxIndex + 3].InitTexture("textures/mp_emerald/emerald-summit_lf.tga", GL_TEXTURE_2D)) {
		cout << "Failed to init emerald texture." << endl;
	}

	menuButtons[numButtons - 1].InitializeGameText("Choose your location", vec3(-0.7, 0.4f, 0), vec3(0), kerning);
	menuButtons[numButtons - 1].SetScale(vec3(2.f));
	//gererate for buttons
	for (int i = 0; i < numButtons - 1; i++)
		menuButtons[i].GenerateSquareVertices(0.2, 0.2, menuButtons[i].getColour());

	float skybox_y = 0.f;

	menuButtons[skyboxIndex].SetPosition(vec3(-0.6, skybox_y, 0));
	menuButtons[skyboxIndex + 1].SetPosition(vec3(-0.2, skybox_y, 0));
	menuButtons[skyboxIndex + 2].SetPosition(vec3(0.2, skybox_y, 0));
	menuButtons[skyboxIndex + 3].SetPosition(vec3(0.6, skybox_y, 0));
	
	for (int i = 0; i < numButtons; i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}

void SkyboxScreen::Run()
{

	switch (KeyCallback(window, controller, audio)) {	//check key callback 
	case 2:
		toggleMenuIndex(1, initIndex, maxIndex);
		break;
	case 3:
		toggleMenuIndex(-1, initIndex, maxIndex);
		break;
		//press "select"
	case 4:
		audio->PlaySfx("armourAndSelect", MIX_MAX_VOLUME, select);
		isVisible = false;
		menuButtons[menuIndex].SetColour(pressColour);	//indicate choice
		menuButtons[menuIndex].SetMixFlag(1);
		break;
		//press "back"
	case 5:
		audio->PlaySfx("back", MIX_MAX_VOLUME, select);
		backPress = true;
		isVisible = false;
		break;
	}
}
