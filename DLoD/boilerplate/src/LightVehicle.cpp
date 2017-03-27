#include "GEO\vehicle\LightVehicle.h"



LightVehicle::LightVehicle()
{
	filename = "cars/lightCarBody.obj";
	maxVelocity = 80;
	health = 100;
	armour = 20;
	armourFilename = "armour/LightArmour.obj";
	initArmour = 20;
	xoff = 2.1f;
	zoff = 5;
	centeroff = 1.0;
}


LightVehicle::~LightVehicle()
{
}
