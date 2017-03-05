#include "GEO\player\Player.h"



Player::Player(int i)
{
  identifier = i;
  playerCam = new Camera();
}


Player::~Player()
{
  delete playerCam;
}

bool Player::isDead() {
	return vehicle->isDead();
}

void Player::getInput()
{

}