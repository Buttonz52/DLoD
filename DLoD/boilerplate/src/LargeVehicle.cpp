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
	xoff = 2.3f;
	zoff = 3.4;
	centeroff = 0.1;
}


LargeVehicle::~LargeVehicle()
{
}
