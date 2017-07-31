#ifndef HUMAN_H
#define HUMAN_H
#include "Player.h"
#include "../../Controller/XboxController.h"

class Human :
	public Player
{
public:
	Human(int, Audio *);
	~Human();
	void setNumCams(const int & n);
	int getNumCams();
	Audio *audio;
	bool pausePressed,restart, menuItemPressed;
	int camIndex, numCams;	//which camera player wants to look through, total number of available cameras in game
	Mix_Chunk *click, *back, *horn;
  XboxController* controller;

  void getInput(GLFWwindow * window, bool & pause);
  void getGameOverInput(GLFWwindow*, bool&);
  void vehicleControls(GLFWwindow * window, bool & pause);
  void gameOverControls(GLFWwindow*, bool&);
  bool pressedPause();

  bool restartGame();

};
#endif