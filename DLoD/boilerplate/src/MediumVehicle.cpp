#include "GEO\vehicle\MediumVehicle.h"



MediumVehicle::MediumVehicle()
{
	xoff = 2.2f;	//this is not good
	zoff = 5.5;		//this is not good
	filename = "cars/mediumCarBody.obj";
	maxVelocity = 90;
	health = 110;
	armour = 30;
	torqueSpeed = 13000.0;
	armourFilename = "armour/MediumArmour.obj";
	initArmour = 30;
}


MediumVehicle::~MediumVehicle()
{
}
