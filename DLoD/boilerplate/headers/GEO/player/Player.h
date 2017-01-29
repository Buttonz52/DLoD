#ifndef PLAYER_H
#define PLAYER_H
#include "..\vehicle\Vehicle.h"
#include "..\Camera.h"
class Player
{
public:
	Player();
	~Player();
	Vehicle vehicle;
	Camera playerCam;

	void GetInput();
};
#endif
