#ifndef SCREEN_H
#define SCREEN_H
#include "Game/Utility.h"
#include "Game/Audio.h"
#include "Game/ScreenOverlay.h"
#include "Controller/XboxController.h"

//Use this class to extend and fix the crappy title screen code
class Screen {
public:
	bool isVisible;
	Screen();
	Screen(GLFWwindow * w, XboxController * x, Audio * a, vec3 & c);
	virtual ~Screen();
	virtual void Initialize();
	virtual void Render();
	virtual void Destroy();
	virtual void Run();
	void UpdateColour(const vec3 & newColour);
	bool checkVisible();
	void toggleMenuIndex(const int & s, const int & initIndex, const int & maxIndex);
	int KeyCallback(GLFWwindow* window, XboxController *ctrller, Audio *audio);
	virtual int returnIndex();
	int getScreenIndex();
	bool goBack();

protected:
	GLFWwindow *window;
	XboxController *controller;
	Audio *audio;
	int initIndex;
	int maxIndex;
	vector<ScreenOverlay> menuButtons;
	vec3 selectColour;
	vec3 pressColour;
	vec3 prevColour;
	vec3 textColour;
	int menuIndex;
	int kerning;
	int screenIndex;
	bool backPress;
};

#endif
