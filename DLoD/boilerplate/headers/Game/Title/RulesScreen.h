#pragma once
#include "Screen.h"
class RulesScreen :
	public Screen
{
public:
	RulesScreen();
	RulesScreen(GLFWwindow * w, XboxController * x, Audio * a);
	~RulesScreen();

	void Initialize();
	void Run();
	//int KeyCallback(GLFWwindow* window, XboxController *ctrller, Audio *audio);
	//void returnValue(void *);
private:
	int pauseTime;
	int timeCounter;
};

