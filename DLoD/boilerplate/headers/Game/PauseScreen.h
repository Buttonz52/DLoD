#pragma once
#include "Game\Title\Screen.h"
class PauseScreen :
	public Screen
{
public:
	PauseScreen();
	PauseScreen(GLFWwindow * w, XboxController * x, Audio * a, vec3 & c);
	int KeyCallback(GLFWwindow * w, XboxController * c, Audio * a);
	~PauseScreen();
	void Initialize();
	void Run();
	bool isRestart, isQuit;

};

