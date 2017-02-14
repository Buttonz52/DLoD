#include "GEO\vehicle\Vehicle.h"



Vehicle::Vehicle()
{

}


Vehicle::~Vehicle()
{
	audio.CleanUp();
}


void Vehicle::accelerate(float m)
{
  physXVehicle->setDriveTorque(0, m*1000.0f);
  physXVehicle->setDriveTorque(1, m*1000.0f);
  physXVehicle->setDriveTorque(2, m*1000.0f);
  physXVehicle->setDriveTorque(3, m*1000.0f);
}


void Vehicle::decelerate(float m)
{
  physXVehicle->setDriveTorque(0, m*-1000.0f);
  physXVehicle->setDriveTorque(1, m*-1000.0f);
  physXVehicle->setDriveTorque(2, m*-1000.0f);
  physXVehicle->setDriveTorque(3, m*-1000.0f);
}

void Vehicle::turn(float dir)
{
  physXVehicle->setSteerAngle(0, dir / 4);
  physXVehicle->setSteerAngle(1, dir / 4);
}

void Vehicle::brake(float brake)
{
  physXVehicle->setBrakeTorque(0, brake);
  physXVehicle->setBrakeTorque(1, brake);
  physXVehicle->setBrakeTorque(2, brake);
  physXVehicle->setBrakeTorque(3, brake);
}

void Vehicle::releaseAllControls()
{
  physXVehicle->setDriveTorque(0, 0.0f);
  physXVehicle->setDriveTorque(1, 0.0f);
  physXVehicle->setDriveTorque(2, 0.0f);
  physXVehicle->setDriveTorque(3, 0.0f);

  physXVehicle->setBrakeTorque(0, 0.0f);
  physXVehicle->setBrakeTorque(1, 0.0f);
  physXVehicle->setBrakeTorque(2, 0.0f);
  physXVehicle->setBrakeTorque(3, 0.0f);

  physXVehicle->setSteerAngle(0, 0.0f);
  physXVehicle->setSteerAngle(1, 0.0f);
  physXVehicle->setSteerAngle(2, 0.0f);
  physXVehicle->setSteerAngle(3, 0.0f);
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

void Vehicle::playSFX()
{
	audio.PlaySfx(crash);
}
