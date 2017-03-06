#ifndef PLAYER_H
#define PLAYER_H
#include "..\vehicle\Vehicle.h"
#include "..\Camera.h"
class Player
{
public:
	Player(int);
	~Player();

  int identifier;
	
	Vehicle* vehicle;
	Camera* playerCam;

	virtual void getInput();
	bool isDead();

  bool layTrap = false;
  bool ableToTrap = true;
};
#endif
