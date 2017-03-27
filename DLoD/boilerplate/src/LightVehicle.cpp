#include "GEO\vehicle\LightVehicle.h"



LightVehicle::LightVehicle()
{
	filename = "cars/lightCarBody.obj";
	maxVelocity = 80;
	health = 100;
	armour = 20;
	torqueSpeed = 14000.0;
	armourFilename = "armour/LightArmour.obj";
	initArmour = 20;
}


LightVehicle::~LightVehicle()
{
}
