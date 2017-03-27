#include "GEO\vehicle\LightVehicle.h"



LightVehicle::LightVehicle()
{
	filename = "cars/lightCarBody.obj";
	maxVelocity = 80;
	health = 100;
	armour = 20;
	armourFilename = "armour/LightArmour.obj";
	initArmour = 20;
	xoff = 2.f;
	zoff = 3.9;
}


LightVehicle::~LightVehicle()
{
}
