#include "GEO\player\Player.h"



Player::Player()
{
}


Player::~Player()
{
}

bool Player::isDead() {
	return vehicle->isDead();
}