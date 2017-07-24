#pragma once
#include "Screen.h"
class SkyboxScreen :
	public Screen
{
public:
	SkyboxScreen();
	SkyboxScreen(GLFWwindow * w, XboxController * x, Audio * a);
	~SkyboxScreen();
	void Initialize();
	void Run();

};

