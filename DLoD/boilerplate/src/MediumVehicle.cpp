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
	xoff = 2.f;
	zoff = 3.6;
	centeroff = -0.5;
}


MediumVehicle::~MediumVehicle()
{
}
