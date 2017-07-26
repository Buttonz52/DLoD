#include "..\headers\Game\Title\ArenaScreen.h"



ArenaScreen::ArenaScreen() :Screen() 
{
	maxIndex = 2;
	screenIndex = 5;
	kerning = 30;
};

ArenaScreen::ArenaScreen(GLFWwindow * w, XboxController * x, Audio * a) :Screen(w,x,a, vec3())
{
	maxIndex = 2;
	screenIndex = 5;
	kerning = 30;
};

ArenaScreen::~ArenaScreen()
{
}

void ArenaScreen::Run()
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
		audio->PlaySfx("armourAndSelect", MIX_MAX_VOLUME,select);
		menuButtons[menuIndex].SetColour(pressColour);	//indicate choice
		menuButtons[menuIndex].SetMixFlag(1);
		isVisible = false;

		break;
		//press "back"
	case 5:
		audio->PlaySfx("back", MIX_MAX_VOLUME,select);
		//reset to choose arena if choosing skybox
		backPress = true;
		isVisible = false;
		break;
	}
	
}

void ArenaScreen::Initialize()
{
	int numButtons = 4;
	int arenaIndex = 0;
	//vector of buttons
	for (int i = 0; i < numButtons; i++)
		menuButtons.emplace_back();
		
	//initialize which button cursor will be on upon starting
	menuButtons[arenaIndex].SetColour(selectColour);	//is set to green initially
	menuButtons[arenaIndex].SetMixFlag(1); //init
	prevColour = vec3(1, 1, 0);	//colour for first button
		
	//init transparent arena textures
	if (!menuButtons[arenaIndex].InitTexture("textures/arenaImgs/bowl.png", GL_TEXTURE_2D))
		cout << "Failed to init bowl texture." << endl;
	
	if (!menuButtons[arenaIndex + 1].InitTexture("textures/arenaImgs/devil.png", GL_TEXTURE_2D))
		cout << "Failed to init devil texture." << endl;
	
	if (!menuButtons[arenaIndex + 2].InitTexture("textures/arenaImgs/wrap.png", GL_TEXTURE_2D))
		cout << "Failed to init wrap texture." << endl;
	

	//screen overlays for arena and location
	menuButtons[3].InitializeGameText("Choose your arena", vec3(-0.6, 0.4, 0), vec3(0), kerning);
	menuButtons[3].SetScale(vec3(2.f));
	//gererate for buttons
	for (int i = 0; i < numButtons-1; i++) 
		menuButtons[i].GenerateSquareVertices(0.2, 0.2, menuButtons[i].getColour());
	
	float arena_y = 0.f;
	//set positions of buttons
	menuButtons[arenaIndex].SetPosition(vec3(-0.4, arena_y, 0));
	menuButtons[arenaIndex + 1].SetPosition(vec3(0.0, arena_y, 0));
	menuButtons[arenaIndex + 2].SetPosition(vec3(0.4, arena_y, 0));
	
	for (int i = 0; i < numButtons; i++)
		menuButtons[i].InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
}
