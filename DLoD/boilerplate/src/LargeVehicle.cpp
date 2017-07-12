#include "GEO\vehicle\LargeVehicle.h"



LargeVehicle::LargeVehicle(Audio *audio, const playerType &type)
{
	this->type = type;
	this->audio = audio;
	filename = "cars/largeCarBody.obj";
	maxVelocity = 60;
	health = 120;
  initHealth = 120;
	armour = 40;
	armourFilename = "armour/LargeArmour.obj";
	initArmour = 40;
	xoff = 4.4f;
	zoff = 5.9;
	centeroff = 0.25;
}


LargeVehicle::~LargeVehicle()
{
}
