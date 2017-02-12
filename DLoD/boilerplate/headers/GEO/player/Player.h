#ifndef PLAYER_H
#define PLAYER_H
#include "..\vehicle\Vehicle.h"
//#include "..\Camera.h"
class Player :public GEO
{
public:
	Player();
	Player(vec3 &pos);
	~Player();
	Vehicle vehicle;
	//Camera playerCam;

	void GetInput();
};
#endif
