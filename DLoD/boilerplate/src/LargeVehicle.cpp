#include "GEO\vehicle\LargeVehicle.h"



LargeVehicle::LargeVehicle()
{
	xoff = 2.5f;	//this is good
	zoff = 4.f;	//this is not good
	filename = "cars/largeCarBody.obj";
	maxVelocity = 60;
	health = 120;
	armour = 40;
	armourFilename = "armour/LargeArmour.obj";
	initArmour = 40;
}


LargeVehicle::~LargeVehicle()
{
}
