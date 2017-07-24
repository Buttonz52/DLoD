#pragma once
#include "Screen.h"
class MainScreen :
	public Screen
{
public:
	MainScreen();
	MainScreen(GLFWwindow * w, XboxController * x, Audio * a);
	~MainScreen();
	void Run();
	bool checkRules();
	void Initialize();
	bool checkQuit();
private:
	bool quit;
	bool rulesScreen;

};

