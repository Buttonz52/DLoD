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

	bool RenderMesh(mat4 winRatio, vec3 lightSource, int width, int height);

	// handles keyboard input events
	
	void GetInput();
};
#endif
