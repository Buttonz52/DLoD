#ifndef PLAYER_H
#define PLAYER_H
#include "..\vehicle\Vehicle.h"
#include "../vehicle/LargeVehicle.h"
#include "../vehicle/MediumVehicle.h"
#include "../vehicle/LightVehicle.h"
#include "..\Camera.h"
#include "..\item\Item.h"
class Player
{
public:
	Player(int);
	~Player();

  int identifier;
	
	Vehicle* vehicle;
	Camera* playerCam;

	virtual void getInput();
	void ChooseVehicle(int choice);
	bool isDead();
  bool layTrap = false;
  bool ableToTrap = true;
  ItemType trap;
private: 
	void SetPlayerColour();
	vec3 colour;

};
#endif
