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
#include "Physics\ContactModifyCallback.h"
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

class PhysXMain
{
private:
  map<PxRigidBody*, Vehicle*> vMap;

public:
	PhysXMain();
	~PhysXMain();

	void init();
	VehicleDesc initVehicleDesc();
	void initVehicle(Vehicle* v);
	void initObject(GEO* g);

  void collisionFunction(PxContactModifyPair* const, PxU32);

	void stepPhysics(bool interactive, vector<GEO *> g);
	void cleanupPhysics(bool interactive);

	mat4 convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w);
};

#endif