#ifndef PLAYER_H
#define PLAYER_H
#include "GEO\vehicle\Vehicle.h"
#include "GEO\vehicle\LargeVehicle.h"
#include "GEO\vehicle\MediumVehicle.h"
#include "GEO\vehicle\LightVehicle.h"
#include "GEO\Camera.h"
#include "GEO\item\Item.h"
class Player
{
public:
	Player(int);
	virtual ~Player();

  int identifier;
	
	Vehicle* vehicle;
	Camera* playerCam;

	virtual void getInput();
	void ChooseVehicle(int choice);
	bool isDead();
	vec3 * getColour();

  bool layTrap = false;
  bool ableToTrap = true;
  bool ableToFlip = false;
  ItemType trap;
private: 
	void SetPlayerColour();
	vec3 colour;
};
#endif
