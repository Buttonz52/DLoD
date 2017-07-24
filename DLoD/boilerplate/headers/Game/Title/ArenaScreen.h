#pragma once
#include "Screen.h"
class ArenaScreen :
	public Screen
{
public:
	ArenaScreen();
	ArenaScreen(GLFWwindow * w, XboxController * x, Audio * a);

	~ArenaScreen();

	void Run();
	void Initialize();
};

