#ifndef VEHICLE_H
#define VEHICLE_H
#include "..\GEO.h"
#include "..\item\Item.h"
#include "Physics\PhysXMain.h"

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
	Item items[3];	//arbitrarily at 3; make this a vector?
	vec3 colour;


	double mass;
	double acceleration;
	double turnRate;
	double maxVelocity;

	float wheelMass;
	float wheelWidth;
	float wheelRadius;
	float numWheels;
	float wheelMOI;
	//PxMaterial* wheelMaterial; //add this here?

	float chassisMass;
	vec3 chassisDims;
	vec3 chassisMOI;
	vec3 chassisCMOffset;
	//PxMaterial *chassisMaterial; //add this here?

	vector <GEO> wheels;
	vector <GEO> armour;
	
	bool IsStunned();


};

#endif