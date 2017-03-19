#include "..\headers\GEO\vehicle\MediumVehicle.h"



MediumVehicle::MediumVehicle()
{
	filename = "cars/mediumCarBody.obj";
	maxVelocity = 70;
	health = 110;
	armour = 30;
	torqueSpeed = 13000.0;
	lowHealth = health / 2;
}


MediumVehicle::~MediumVehicle()
{
}
