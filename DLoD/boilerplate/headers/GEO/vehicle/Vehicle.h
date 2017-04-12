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
  float initHealth;
  float armour;
  float initArmour;
  int lowHealth;
  vec3 initColour;
  string filename;
  string armourFilename;
 
  // Stores the rate of acceleration and maxVelocity
  double acceleration;
  double maxVelocity;
  float torqueSpeed;
  float xoff;
  float zoff;
  float centeroff;
  clock_t timeOfDeath;
  clock_t flipTime;

public:
	Vehicle();
	virtual ~Vehicle();

  PxVehicleNoDrive* physXVehicle;
  pair<bool, double> stun = make_pair(false, 0);
  pair<bool, double> recentlyHit = make_pair(false, 0);
  bool canPulseColour, ableToFlip;
  Timer timer;
  void setColour(const vec3 &col);

  vec3 getOrientationVector();


  string wheelFileName;
  void updateArmour();
  double getInitialArmour();
  void updateWheelPosition();
  void giveMeWheels();

  void Render(const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource);
  void RenderShadow(const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource);
  mat4 convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w);

  void checkEnableFlip();

  void FlipVehicle();

  // Methods to control driving the vehicle
  void accelerate(const float &m);
  void decelerate(const float &m);
  void turn(const float &dir);
  void brake(const float &brake);
  void releaseAllControls();
  void checkDead();
  bool initBuffers();
  bool isDead();
  bool initMesh(const string &);

  void giveMeArmour(const vec3 & colour);

	Mix_Chunk *crash, *explosion;

	float getHealth();
	float calculateDamage(const double &, const double &, const double &, const double &);
	void regenArmour();
	void updateHealth(const float &damage);
  void heal(const float &healthAdded);
	float getArmour();
  void repair(const float &armourAdded);
	string getHealthString();
	string getArmourString();

	string getVelocityString();
	int getTimeOfDeath();
};

#endif