#include "GEO\vehicle\MediumVehicle.h"



MediumVehicle::MediumVehicle()
{
	filename = "cars/mediumCarBody.obj";
	maxVelocity = 70;
	health = 110;
	armour = 30;
	torqueSpeed = 13000.0;
	armourFilename = "armour/MediumArmour.obj";
	initArmour = 30;
}


MediumVehicle::~MediumVehicle()
{
}
