#pragma once
#include "Screen.h"
class MultiplayerScreen :
	public Screen
{
public:
	MultiplayerScreen();
	MultiplayerScreen(GLFWwindow * w, XboxController * x, Audio * a);
	~MultiplayerScreen();
	void Run();
	void Initialize();
	int returnIndex();
};

