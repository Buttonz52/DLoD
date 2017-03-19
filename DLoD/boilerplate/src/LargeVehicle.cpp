#include "..\headers\GEO\vehicle\LargeVehicle.h"



LargeVehicle::LargeVehicle()
{
	filename = "cars/largeCarBody.obj";
	maxVelocity = 60;
	health = 120;
	armour = 40;
	torqueSpeed = 12000.0;
	lowHealth = health / 2;
}


LargeVehicle::~LargeVehicle()
{
}
