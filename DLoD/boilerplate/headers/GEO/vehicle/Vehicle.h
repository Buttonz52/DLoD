#ifndef VEHICLE_H
#define VEHICLE_H
#include "..\GEO.h"
#include "..\item\Item.h"
#include "..\..\Game\Audio.h"

using namespace physx;

class Vehicle :
	public GEO
{
private:


  // Stores the Vehicle Health
  float health = 1000;
  double armourLeft;
  double armourRight;
  double armourFront;
  double armourBack;

  // Stores the rate of acceleration and maxVelocity
  double acceleration;
  double maxVelocity;


public:
	Vehicle();
	~Vehicle();

  PxVehicleNoDrive* physXVehicle;

  // Methods to control driving the vehicle
  void accelerate(float m);
  void decelerate(float m);
  void turn(float dir);
  void brake(float brake);
  void releaseAllControls();

	Audio audio;

	vec3 colour;

	Mix_Chunk* crash = Mix_LoadWAV("sfx/carCrash.wav");

	float getHealth();
	float calculateDamage();
	void updateHealth(float damage);
	void playSFX();

};

#endif