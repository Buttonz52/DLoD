#ifndef VEHICLE_H
#define VEHICLE_H
#include "..\GEO.h"
#include "..\..\Game\timer.h"

using namespace physx;

class Vehicle :
	public GEO
{
protected:

  bool dead;
  // Stores the Vehicle Health
  float health;
  float armour;
  float initArmour;
  int lowHealth;
  vec3 initColour;
  string filename;
  string armourFilename;
  //double armourLeft;
  //double armourRight;
  //double armourFront;
  //double armourBack;
 
  // Stores the rate of acceleration and maxVelocity
  double acceleration;
  double maxVelocity;
  float torqueSpeed;
  
  Mesh aliveCar, deadCar;

public:
	Vehicle();
	virtual ~Vehicle();

  PxVehicleNoDrive* physXVehicle;
  pair<bool, double> stun = make_pair(false, 0);
  bool canPulseColour;
  Timer timer;
  void setColour(const vec3 &col);


  string wheelFileName;
  //void updateArmourPosition();
  void updateArmour();
  void updateWheelPosition();
  //void giveMeArmour();
  void giveMeWheels();

  void Render(const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource);

  mat4 convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w);

  void FlipVehicle();

  // Methods to control driving the vehicle
  void accelerate(const float &m);
  void decelerate(const float &m);
  void turn(const float &dir);
  void brake(const float &brake);
  void releaseAllControls();
  void checkDead();
  bool initBuffers();
  void changeMeshDead();
  string & getAliveCarMesh();
  bool isDead();
  bool initMesh(const string &);

  void giveMeArmour(const vec3 & colour);

	Mix_Chunk *crash, *explosion;

	float getHealth();
	float calculateDamage(const double &, const double &, const double &, const double &);
	void setAliveCarMesh(const string & file);
	void regenArmour();
	void updateHealth(const float &damage);
	float getArmour();
	string getHealthString();
	string getArmourString();

	string getVelocityString();

};

#endif