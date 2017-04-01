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
	void setNumCams(const int & n);
	int getNumCams();
	bool pausePressed,restart, menuItemPressed;
	int camIndex, numCams;	//which camera player wants to look through, total number of available cameras in game
	Mix_Chunk *click, *back;
  XboxController* controller;

  void getInput(GLFWwindow * window, bool & pause);
  void getGameOverInput(GLFWwindow*, bool&);
  void vehicleControls(GLFWwindow * window, bool & pause);
  void menuControls(GLFWwindow * window, bool & pause, int & index, Audio * audio);
  void gameOverControls(GLFWwindow*, bool&);
  bool MenuItemSelected();
  bool pressedPause();

  bool restartGame();


};
#endif