#include "GEO\player\Player.h"



Player::Player()
{
}

Player::Player(vec3 &pos)
{
	vehicle.setPosition(pos);
	position = pos;
	cout << "Player pos: "<<position.x << endl;
	//position = pos;
}

Player::~Player()
{
}

void Player::GetInput()
{
}
