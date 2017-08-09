#include "..\headers\Game\Title\Screen.h"

Screen::Screen()
{
	menuIndex = 0;
	isVisible = true;
	selectColour = vec3(1, 0.5, 0.5);
	pressColour = vec3(1, 0, 1);
	prevColour = vec3(0);
	screenIndex = -1;
	initIndex = 0;
	maxIndex = 0;
	menuIndex = 0;
	backPress = false;
	kerning = 30;
	textColour = vec3();
	inputPause = 150;
}

Screen::Screen(GLFWwindow *w, XboxController *x, Audio *a, vec3 &c) {
	window = w;
	controller = x;
	audio = a;
	isVisible = true;
	selectColour = vec3(1, 0.5, 0.5);
	pressColour = vec3(1, 0, 1);
	prevColour = c;
	screenIndex = -1;
	initIndex = 0;
	maxIndex = 0;
	menuIndex = 0;
	backPress = false;
	kerning = 30;
	textColour = c;
	inputPause = 1500;
}

Screen::~Screen()
{
}

void Screen::Initialize()
{
	if (!timer.isStarted())
		timer.start();
}

void Screen::Run() {
	if (timer.checkSleep())
		return;
}

void Screen::UpdateColour(const vec3 &newColour) {
	prevColour = newColour;
	for (int i = 0; i < menuButtons.size(); i++) {
		if (menuButtons[i].GetMixFlag() != 1)
			menuButtons[i].SetColour(newColour);
	}
}

void Screen::Render()
{
	for (int i = 0; i < menuButtons.size(); i++)
		if (!menuButtons[i].isFontTex) {
			menuButtons[i].Render(GL_TRIANGLE_STRIP, menuButtons[i].getColour());	//render regular button
		}
		else {
			menuButtons[i].Render(GL_TRIANGLES, menuButtons[i].getColour());	//render text, as it uses triangles, not triangle strip
		}
}

void Screen::Destroy()
{
	for (int i = 0; i < menuButtons.size(); i++)
		menuButtons[i].Destroy();
}

int Screen::KeyCallback(GLFWwindow * window, XboxController * ctrller, Audio * audio)
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
	return -1;
}

int Screen::returnIndex() {
	return menuIndex;
}
int Screen::getScreenIndex()
{
	return screenIndex;
}

bool Screen::checkVisible() {
	return isVisible;
}

//change buttons
void Screen::toggleMenuIndex(const int &s, const int &initIndex, const int &maxIndex) {
	menuButtons[menuIndex].SetColour(prevColour);	//button back to normal colour
	menuButtons[menuIndex].SetMixFlag(0);	//no longer mix colour
	menuIndex += s;

	//keep index in bounds
	if (menuIndex > maxIndex)
		menuIndex = initIndex;
	if (menuIndex < initIndex)
		menuIndex = maxIndex;

	//set the previous colour
	prevColour = menuButtons[menuIndex].getColour();
	//new colour (green for the moment); used to mix with texture for current button (we can change this)
	menuButtons[menuIndex].SetColour(selectColour);
	//sex mixColour flag
	menuButtons[menuIndex].SetMixFlag(1);
	audio->PlaySfx("toggle", MIX_MAX_VOLUME, menu);
}

bool Screen::goBack() {
	return backPress;
}