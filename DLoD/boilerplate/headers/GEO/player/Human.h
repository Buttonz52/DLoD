#ifndef HUMAN_H
#define HUMAN_H
#include "Player.h"
#include "../../Controller/XboxController.h"
class Human :
	public Player
{
public:
	Human();
	~Human();

  XboxController* controller;

  void getInput(GLFWwindow*);
  void vehicleControls(GLFWwindow*);
  void menuControls(GLFWwindow*);
};
#endif