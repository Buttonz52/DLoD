#include "GEO\vehicle\Vehicle.h"



Vehicle::Vehicle()
{
	//health = 100;
}


Vehicle::~Vehicle()
{
}


bool Vehicle::IsStunned()
{
	return false;
}

float Vehicle::calculateDamage()
{
	float velocity = this->physXVehicle->computeForwardSpeed();
	float mass = this->physXVehicle->getRigidDynamicActor()->getMass();

	float damage = velocity * mass / 15000;

	updateHealth(abs(damage));

	return damage;
}

float Vehicle::getHealth()
{
	return health;
}

void Vehicle::updateHealth(float damage)
{
	health -= damage;
	if (health < 0)
		health = 0;
}

