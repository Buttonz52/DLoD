#include "GEO\player\Player.h"



Player::Player(int i)
{
  identifier = i;
  playerCam = new Camera();
  ableToTrap = true;
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