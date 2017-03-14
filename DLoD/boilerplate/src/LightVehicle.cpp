#include "..\headers\GEO\vehicle\LightVehicle.h"



LightVehicle::LightVehicle()
{
	filename = "cars/lightCarBody.obj";
	maxVelocity = 90;
	health = 120;
	armour = 10;
	lowHealth = health / 2;
}


LightVehicle::~LightVehicle()
{
}
