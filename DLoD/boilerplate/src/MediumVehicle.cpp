#include "GEO\vehicle\MediumVehicle.h"



MediumVehicle::MediumVehicle()
{
	xoff = 2.2f;	//this is not good
	zoff = 5.5;		//this is not good
	filename = "cars/mediumCarBody.obj";
	maxVelocity = 70;
	health = 110;
	armour = 30;
	armourFilename = "armour/MediumArmour.obj";
	initArmour = 30;
}


MediumVehicle::~MediumVehicle()
{
}
