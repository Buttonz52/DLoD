#ifndef VEHICLE_H
#define VEHICLE_H
#include "..\GEO.h"
#include "..\item\Item.h"

using namespace physx;

class Vehicle :
	public GEO
{
public:
	Vehicle();
	~Vehicle();

	PxVehicleNoDrive* physXVehicle;


	int health;
	double armourLeft;	
	double armourRight;
	double armourFront;
	double armourBack;
	double mass;
	double acceleration;
	double turnRate;
	double maxVelocity;
	Item items[3];	//arbitrarily at 3; make this a vector?
	vec3 colour;
	vector <GEO> wheels;
	vector <GEO> armour;
	



	bool IsStunned();

};

#endif