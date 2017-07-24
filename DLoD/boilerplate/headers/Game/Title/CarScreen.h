#pragma once
#include "Screen.h"
class CarScreen :
	public Screen
{
public:
	CarScreen();
	CarScreen(GLFWwindow * w, vector<XboxController> &x, Audio * a, const int &numPlayers);
	~CarScreen();

	void Run();
	void Initialize();
private:
	int controllerIndex;
	int numHumans;
	vector<XboxController> controllers;
	//vector<int> humanVehicleChoice;

};

