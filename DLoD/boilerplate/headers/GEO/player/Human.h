#ifndef HUMAN_H
#define HUMAN_H
#include "Player.h"
#include "../../Controller/XboxController.h"
class Human :
	public Player
{
public:
	Human(int);
	~Human();
	bool pausePressed,restart, menuItemPressed;
  XboxController* controller;

  void getInput(GLFWwindow * window, bool & pause);
  void getGameOverInput(GLFWwindow*, bool&);
  //void vehicleControls(GLFWwindow*);
  void vehicleControls(GLFWwindow * window, bool & pause);
  void menuControls(GLFWwindow * window, bool & pause, int & index);
  //void menuControls(GLFWwindow * window, bool & pause);
  void gameOverControls(GLFWwindow*, bool&);
  bool MenuItemSelected();
  bool pressedPause();

  bool restartGame();

};
#endif