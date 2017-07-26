#pragma once
#include "Screen.h"
class MainScreen :
	public Screen
{
public:
	MainScreen();
	MainScreen(GLFWwindow * w, XboxController * x, Audio * a, vec3 &c);
	~MainScreen();
	void Run();
	bool checkRules();
	void Initialize();
	bool checkQuit();
private:
	bool quit;
	bool rulesScreen;

};

