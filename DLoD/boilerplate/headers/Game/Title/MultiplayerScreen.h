#pragma once
#include "Screen.h"
class MultiplayerScreen :
	public Screen
{
public:
	MultiplayerScreen();
	MultiplayerScreen(GLFWwindow * w, XboxController * x, Audio * a, vec3 &c);
	~MultiplayerScreen();
	void Run();
	void Initialize();
	int returnIndex();
};

