#pragma once
#include "Screen.h"
class ModeScreen :
	public Screen
{
public:
	ModeScreen();
	ModeScreen(GLFWwindow * w, XboxController * x, Audio * a);
	~ModeScreen();
	void Run();
	void Initialize();
};

