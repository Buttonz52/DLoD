#include "GEO\vehicle\LightVehicle.h"



LightVehicle::LightVehicle()
{
	filename = "cars/lightCarBody.obj";
	maxVelocity = 80;
	health = 100;
  initHealth = 100;
	armour = 20;
	armourFilename = "armour/LightArmour.obj";
	initArmour = 20;
	xoff = 3.5f;
	zoff = 6.4;
	centeroff = 1.4;
}


LightVehicle::~LightVehicle()
{
}
