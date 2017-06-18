#include "GEO\vehicle\MediumVehicle.h"



MediumVehicle::MediumVehicle()
{
	filename = "cars/mediumCarBody.obj";
	maxVelocity = 70;
	health = 110;
  initHealth = 110;
	armour = 30;
	armourFilename = "armour/MediumArmour.obj";
	initArmour = 30;
	xoff = 3.6f;
	zoff = 5.9;
	centeroff = -1;
}


MediumVehicle::~MediumVehicle()
{
}
