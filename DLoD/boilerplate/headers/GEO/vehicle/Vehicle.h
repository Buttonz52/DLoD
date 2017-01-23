#ifndef VEHICLE_H
#define VEHICLE_H
#include "..\GEO.h"
#include "..\item\Item.h"

class Vehicle :
	public GEO
{
public:
	Vehicle();
	~Vehicle();

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

	
	bool IsStunned();

};

#endif