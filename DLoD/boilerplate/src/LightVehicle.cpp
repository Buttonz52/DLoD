#include "..\headers\GEO\vehicle\LightVehicle.h"



LightVehicle::LightVehicle()
{
	filename = "cars/lightCarBody.obj";
	armourFilename = "armour/LightArmour.obj";
	maxVelocity = 90;
	health = 120;
	initArmour = 10;
	armour = 10;
	lowHealth = health / 2;
}


LightVehicle::~LightVehicle()
{
}
