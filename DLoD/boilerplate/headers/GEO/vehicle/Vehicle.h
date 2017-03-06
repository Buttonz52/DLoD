#ifndef VEHICLE_H
#define VEHICLE_H
#include "..\GEO.h"

using namespace physx;

class Vehicle :
	public GEO
{
private:


	bool dead;
  // Stores the Vehicle Health
  float health;
  float armour;
  //double armourLeft;
  //double armourRight;
  //double armourFront;
  //double armourBack;

  // Stores the rate of acceleration and maxVelocity
  double acceleration;
  double maxVelocity = 70;
  float torqueSpeed;
  
  Mesh aliveCar, deadCar;

public:
	Vehicle();
	~Vehicle();

  PxVehicleNoDrive* physXVehicle;

  string wheelFileName;
  void updateWheelPosition();

  // Methods to control driving the vehicle
  void accelerate(const float &m);
  void decelerate(const float &m);
  void turn(const float &dir);
  void brake(const float &brake);
  void releaseAllControls();
  void checkDead();
  bool initBuffers();
  void changeMeshDead();
  bool isDead();
  bool initMesh(const string &);

	Mix_Chunk* crash;

	float getHealth();
	float calculateDamage(const double &, const double &, const double &, const double &);
	void regenArmour();
	void updateHealth(const float &damage);
	float getArmour();
	string getHealthString();
	string getArmourString();

};

#endif