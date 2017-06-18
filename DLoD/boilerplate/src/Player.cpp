#include "GEO\player\Player.h"

Player::Player(int i)
{
  identifier = i;
  SetPlayerColour();	//Colours initialized here based on your player ID (what player you are)
  playerCam = new Camera();
  ableToTrap = true;
  ableToFlip = true;

}

Player::~Player()
{
  delete playerCam;
}

int Player::getTimeOfDeath() {
	return vehicle->getTimeOfDeath();
}
//sets the player colour based on your identifier number.
void Player::SetPlayerColour() {
	switch (identifier) {
	case 0:
		colour = vec3(1, 0, 0);		//red
		break;
	case 1:
		colour = vec3(0, 1, 0);		//green
		break;
	case 2:
		colour = vec3(0, 0, 1);		//blue
		break;
	case 3:
		colour = vec3(1, 1, 0);		//yellow
		break;
	case 4:
		colour = vec3(1, 0.5, 0);	//orange
		break;
	case 5:
		colour = vec3(1, 0.078, 0.58);		//deep pink
		break;
	case 6:
		colour = vec3(0, 1, 1);		//cyan
		break;
	case 7:
		colour = vec3(0.55, 0.028, 0.54);	//orchid4
		break;
	default:
		colour = vec3(0, 0, 0);		//default black
		break;
	}
}
bool Player::isDead() {
	return vehicle->isDead();
}

vec3 * Player::getColour() {
	return &colour;
}
void Player::getInput()
{

}

void Player::ChooseVehicle(int choice) {
	//random choice between vehicle classes
	switch (choice) {
	case 0:
		vehicle = new LightVehicle();
		break;
	case 1:
		vehicle = new MediumVehicle();
		break;
	case 2:
		vehicle = new LargeVehicle();
		break;
	default:
		vehicle = new MediumVehicle();
		break;
	}
	vehicle->setColour(colour);
}
