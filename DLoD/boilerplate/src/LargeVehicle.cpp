#include "..\headers\GEO\vehicle\LargeVehicle.h"



LargeVehicle::LargeVehicle()
{
	filename = "cars/largeCarBody.obj";
	armourFilename = "armour/HeavyArmour.obj";
	maxVelocity = 40;
	health = 40;
	initArmour = 100;
	armour = 100;
	lowHealth = health / 2;
}


LargeVehicle::~LargeVehicle()
{
}
