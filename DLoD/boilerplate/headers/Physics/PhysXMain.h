#ifndef PHYSXMAIN_H
#define PHYSXMAIN_H

#define PVD_HOST "127.0.0.1"

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "../GEO/GEO.h"
#include "../GEO/vehicle/Vehicle.h"
#include "Physics\SnippetVehicleRaycast.h"
#include "Physics\SnippetVehicleCreate.h"
#include "Physics\SnippetVehicleTireFriction.h"
#include "Physics\SnippetVehicleFilterShader.h"
#include <map>

using namespace physx;

struct ContactModFlags
{
	enum
	{
		eADJUST_MASS_RATIOS = 1,
		eIGNORE_CONTACT = 2,
		eMODIFY_NORMAL = 4,
		eADJUST_PENETRATION = 8,
		eMAX_IMPULSE = 16,
		eTARGET_VELOCITY = 32
	};
};

static void adjustMassRatios(const PxRigidDynamic& dynamic0, const PxRigidDynamic& dynamic1, const PxReal maxMassRatio, PxContactModifyPair& pair)
{
	PxReal mass0 = dynamic0.getMass();
	PxReal mass1 = dynamic1.getMass();

	if (mass0 > mass1)
	{
		//dynamic0 is heavier than dynamic1 so we will locally increase the mass of dynamic1 
		//to be half the mass of dynamic0.
		PxReal ratio = mass0 / mass1;
		if (ratio > maxMassRatio)
		{
			PxReal invMassScale = maxMassRatio / ratio;
			pair.contacts.setInvMassScale1(invMassScale);
			pair.contacts.setInvInertiaScale1(invMassScale);
		}
	}
	else
	{
		//dynamic1 is heavier than dynamic0 so we will locally increase the mass of dynamic0 
		//to be half the mass of dynamic1.
		PxReal ratio = mass1 / mass0;
		if (ratio > maxMassRatio)
		{
			PxReal invMassScale = maxMassRatio / ratio;
			pair.contacts.setInvMassScale0(invMassScale);
			pair.contacts.setInvInertiaScale0(invMassScale);
		}
	}
}

static void ignoreContacts(PxContactModifyPair& pair)
{
	for (PxU32 i = 0; i < pair.contacts.size(); ++i)
	{
		pair.contacts.ignore(i);
	}
}

static void rotateNormals(PxContactModifyPair& pair, const PxQuat& rotation)
{
	for (PxU32 i = 0; i < pair.contacts.size(); ++i)
	{
		PxVec3 normal = pair.contacts.getNormal(i);
		pair.contacts.setNormal(i, rotation.rotate(normal));
	}
}

static void allowPenetration(PxContactModifyPair& pair, const PxReal allowedPenetration)
{
	for (PxU32 i = 0; i < pair.contacts.size(); ++i)
	{
		pair.contacts.setSeparation(i, pair.contacts.getSeparation(i) + allowedPenetration);
	}
}

static void setMaxImpulse(PxContactModifyPair& pair, const PxReal maxImpulse)
{
	for (PxU32 i = 0; i < pair.contacts.size(); ++i)
	{
		pair.contacts.setMaxImpulse(i, maxImpulse);
	}
}

static void setTargetVelocity(PxContactModifyPair& pair, const PxVec3& targetVelocity)
{
	for (PxU32 i = 0; i < pair.contacts.size(); ++i)
	{
		pair.contacts.setTargetVelocity(i, targetVelocity);
	}
}



class PhysXMain
{
public:
	PhysXMain();
	~PhysXMain();

	void init();
	VehicleDesc initVehicleDesc();
	void initVehicle(Vehicle* v);
	void initObject(GEO* g);

	void accelerate(Vehicle* g, float m);
	void decelerate(Vehicle* g, float m);
	void turn(Vehicle* v, float dir);
	void brake(Vehicle* v, float brake);
	void releaseAllControls(Vehicle* v);

	void stepPhysics(bool interactive, vector<GEO *> g);
	void cleanupPhysics(bool interactive);

	mat4 convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w);
};

#endif