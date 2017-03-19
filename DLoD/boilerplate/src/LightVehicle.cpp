#include "..\headers\GEO\vehicle\LightVehicle.h"



LightVehicle::LightVehicle()
{
	filename = "cars/lightCarBody.obj";
	maxVelocity = 80;
	health = 100;
	armour = 20;
	torqueSpeed = 14000.0;
	lowHealth = health / 2;
}


LightVehicle::~LightVehicle()
{
}
