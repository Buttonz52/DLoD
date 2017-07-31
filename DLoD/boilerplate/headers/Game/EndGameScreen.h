#pragma once
#include "title/Screen.h"
class EndGameScreen : public Screen
{
public:
	EndGameScreen();
	EndGameScreen(GLFWwindow * w, XboxController * x, Audio * a, vec3 & c, const vector<string>&, const vector<vec3>&);
	~EndGameScreen();
	void Initialize();
	void Run();
	bool isRestart, isQuit;
private:
	vector<string> playerNames;
	vector<vec3> playerColours;
};

