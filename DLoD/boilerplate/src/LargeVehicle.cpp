#include "GEO\vehicle\LargeVehicle.h"



LargeVehicle::LargeVehicle()
{
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
