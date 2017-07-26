#pragma once
#include "Screen.h"
class PlayButtonScreen :
	public Screen
{
public:
	PlayButtonScreen();
	PlayButtonScreen(GLFWwindow *, XboxController *, Audio *, vec3 &c);
	~PlayButtonScreen();
	void Run();
	void Initialize();
	int returnIndex();
};

